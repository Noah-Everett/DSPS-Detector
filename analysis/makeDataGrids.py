#!/usr/bin/env python3
"""
makeDataGrids.py

Script to process ROOT files to generate DataFrames of hit and primary data,
create voxel grids for ML, split data into train/val/test, and optionally train a UNet model.
"""
import argparse
import os
import sys
import numpy as np
import pandas as pd
import h5py
import uproot
from tqdm import tqdm
from joblib import Parallel, delayed

# Add custom modules to path
sys.path.append('../python/')

# Import custom processing functions
from importMethods import (
    get_histogram_hits_tuple,
    get_histogram_sizes,
    get_histogram_nHits_total,
    get_photosensor_IDs,
    get_photosensor_directions,
    get_photosensor_positions,
    get_photosensor_walls,
    get_photosensor_relativePosition_binned,
    get_photosensor_relativePosition_nBin,
    get_photosensor_initialPosition,
    get_primary_position,
    get_primary_pdg,
)
from filterMethods import make_r, filter_r
from timeMethods import make_theta, make_phi
from trackReconstructionMethods import make_reconstructedVector_direction
from hitAccuracyMethods import make_relativeVector
from gridMethods import get_voxelGrid, make_voxelGrid_truth, expNWalls
from constants import CM_PER_RAD, MM_PER_CM, DETECTOR_SIZE_MM, Y_LIM
from UNetMethods import get_config, save_config
from pytorch3dunet.train import main as train_main


def r_to_theta(r):
    return r / (CM_PER_RAD * MM_PER_CM)


def check_files(paths, grid_shape, hist_dir):
    """
    Verify that each ROOT file contains histograms of the expected shape.
    """
    valid = []
    for path in paths:
        try:
            f = uproot.open(path)
            hists = f[hist_dir]
            for key in hists.keys():
                h = hists[key]
                if h.allnumpy().shape != tuple(grid_shape):
                    print(f"Error: histogram {key} in {path} has shape {h.allnumpy().shape}, expected {tuple(grid_shape)}. Skipping.")
                    raise ValueError
            valid.append(path)
        except Exception as e:
            print(f"Skipping file {path}: {e}")
    return valid


def apply_cuts(paths, min_hits, min_primary_steps, hist_dir, primary_tree):
    """
    Filter files by total hit count and number of primary steps.
    """
    filtered = []
    for path in paths:
        try:
            n_hits = get_histogram_nHits_total(path, directoryName=hist_dir)
            sizes = get_histogram_sizes(path, directoryName=hist_dir)
            prim_positions = get_primary_position(path, primary_tree)
            if n_hits >= min_hits and len(prim_positions) >= min_primary_steps:
                filtered.append(path)
        except Exception as e:
            print(f"Error applying cuts to {path}: {e}")
    return filtered


def process_hits_df(paths, hist_dir, output_base, overwrite, use_histograms):
    """
    Build and save hits DataFrame for each file.
    """
    df_paths = []
    for i, path in enumerate(tqdm(paths, desc="Processing hits DataFrames")):
        out_path = f"{output_base}_{i}.parquet"
        if not overwrite and os.path.exists(out_path):
            df_paths.append(out_path)
            continue
        # Extract data
        if use_histograms:
            ids, dirs, poss, walls, rel_binned, rel_nbin = get_histogram_hits_tuple(
                path, hist_dir, True
            )
            df = pd.DataFrame({
                'sensor_name': ids,
                'sensor_direction': dirs,
                'sensor_position': poss,
                'sensor_wall': walls,
                'relativePosition_binned': rel_binned,
                'relativePosition_nBin': rel_nbin,
            })
        else:
            df = pd.DataFrame({
                'sensor_name': get_photosensor_IDs(path, hist_dir),
                'sensor_direction': get_photosensor_directions(path, hist_dir),
                'sensor_position': get_photosensor_positions(path, hist_dir),
                'sensor_wall': get_photosensor_walls(path, hist_dir),
                'relativePosition_binned': get_photosensor_relativePosition_binned(path, hist_dir),
                'relativePosition_nBin': get_photosensor_relativePosition_nBin(path, hist_dir),
                'initialPosition': get_photosensor_initialPosition(path, hist_treeName, hist_dir)
            })
        # Apply feature engineering
        df = make_r(df)
        df = filter_r(df, Y_LIM)
        df = make_theta(df, r_to_theta)
        df = make_phi(df)
        df = make_reconstructedVector_direction(df)
        if 'initialPosition' in df.columns:
            df = make_relativeVector(df)
        df.to_parquet(out_path, compression='snappy')
        df_paths.append(out_path)
    return df_paths


def process_primary_df(paths, primary_tree, output_base, overwrite):
    """
    Build and save primary DataFrame (muon tracks) for each file.
    """
    df_paths = []
    for i, path in enumerate(tqdm(paths, desc="Processing primary DataFrames")):
        out_path = f"{output_base}_{i}.parquet"
        if not overwrite and os.path.exists(out_path):
            df_paths.append(out_path)
            continue
        positions = get_primary_position(path, primary_tree)
        pdgs = get_primary_pdg(path, primary_tree)
        df = pd.DataFrame({'position': positions, 'pdg': pdgs})
        # Keep only muons (pdg 13) within detector bounds
        df = df[df['pdg'] == 13]
        half = np.array(DETECTOR_SIZE_MM) / 2
        df = df[df['position'].apply(lambda xyz: all(
            -half[j] < xyz[j] < half[j] for j in range(3)
        ))]
        df.to_parquet(out_path, compression='snappy')
        df_paths.append(out_path)
    return df_paths


def create_grids(df_paths_hits, df_paths_primary, paths_npy_hits, paths_npy_primary, paths_h5,
                 save_npy_hits, save_npy_primary, save_h5,
                 grid_shape, combine_errors, combine_walls, walls_combine, walls_method, combine_vectors):
    """
    Create voxel grids from DataFrames and save as .npy and/or .h5.
    """
    def _create_one(args):
        df_hit_path, df_pri_path, npy_hit, npy_pri, h5p = args
        do_npy_hit = save_npy_hits and not (os.path.exists(npy_hit))
        do_npy_pri = save_npy_primary and not (os.path.exists(npy_pri))
        do_h5 = save_h5 and not (os.path.exists(h5p))
        if not (do_npy_hit or do_npy_pri or do_h5):
            return
        # Load and build hit grid
        x = None
        if save_npy_hits or save_h5:
            dfh = pd.read_parquet(df_hit_path)
            x = get_voxelGrid(
                grid_shape=np.array(grid_shape, int),
                grid_dimensions=np.array(DETECTOR_SIZE_MM),
                vector_starts=np.array(dfh['sensor_position'].tolist()).reshape(-1, 3),
                vector_directions=-np.array(dfh['reconstructedVector_direction'].tolist()).reshape(-1, 3),
                vector_weights=None,
                make_errors=combine_errors,
                make_walls=combine_walls,
                make_walls_combine=walls_combine,
                walls_combine_method=walls_method,
                combine_vectors=combine_vectors
            )
        # Load and build primary truth grid
        y = None
        if save_npy_primary or save_h5:
            dfp = pd.read_parquet(df_pri_path)
            y = make_voxelGrid_truth(
                np.array(dfp['position'].tolist()),
                shape=grid_shape,
                detectorDimensions=DETECTOR_SIZE_MM,
                makeErrors=False
            )[0]
        # Save outputs
        if do_h5:
            with h5py.File(h5p, 'w') as f:
                if x is not None:
                    f.create_dataset('x', data=np.moveaxis(x, -1, 0))
                if y is not None:
                    f.create_dataset('y', data=y)
        if do_npy_hit and (x is not None):
            np.save(npy_hit, x)
        if do_npy_pri and (y is not None):
            np.save(npy_pri, y)

    jobs = [ (dfh, dfp, nh, np, h5) for dfh, dfp, nh, np, h5 in 
             zip(df_paths_hits, df_paths_primary, paths_npy_hits, paths_npy_primary, paths_h5) ]
    Parallel(n_jobs=-1)(delayed(_create_one)(j) for j in tqdm(jobs, desc="Creating voxel grids"))


def split_data(all_h5_paths, n_test, n_val, seed):
    """Split dataset indices into train, test, val and return path lists."""
    np.random.seed(seed)
    total = len(all_h5_paths)
    indices = np.arange(total)
    test_idx = np.random.choice(indices, size=n_test, replace=False)
    remaining = np.setdiff1d(indices, test_idx)
    val_idx = np.random.choice(remaining, size=n_val, replace=False)
    train_idx = np.setdiff1d(remaining, val_idx)
    paths_train = [all_h5_paths[i] for i in train_idx]
    paths_test = [all_h5_paths[i] for i in test_idx]
    paths_val = [all_h5_paths[i] for i in val_idx]
    return paths_train, paths_test, paths_val


def main():
    parser = argparse.ArgumentParser(description='Generate ML data grids and optionally train UNet')
    parser.add_argument('input_dir', help='Directory of input ROOT files')
    parser.add_argument('output_dir', help='Base output directory')
    parser.add_argument('--gridSize', type=int, nargs=3, default=[80,80,80], help='Voxel grid size')
    parser.add_argument('--minNHits', type=int, default=0)
    parser.add_argument('--minPrimarySteps', type=int, default=30)
    parser.add_argument('--doCuts', action='store_true', help='Apply hit/step cuts')
    parser.add_argument('--checkFiles', action='store_true', help='Verify histogram shapes')
    parser.add_argument('--useHistograms', action='store_true', help='Use histogram-based hit tuple')
    parser.add_argument('--overwriteDF', action='store_true', help='Overwrite existing DataFrames')
    parser.add_argument('--saveDFhits', action='store_true')
    parser.add_argument('--saveDFprimary', action='store_true')
    parser.add_argument('--saveGridNpyHits', action='store_true')
    parser.add_argument('--saveGridNpyPrimary', action='store_true')
    parser.add_argument('--saveGridH5', action='store_true')
    parser.add_argument('--nTest', type=int, default=20)
    parser.add_argument('--nVal', type=int, default=10)
    parser.add_argument('--seed', type=int, default=42)
    parser.add_argument('--train', action='store_true', help='Train UNet after data prep')
    parser.add_argument('--numWorkers', type=int, default=10)
    parser.add_argument('--checkpointDir', type=str, help='Directory for model checkpoints')
    args = parser.parse_args()

    os.makedirs(args.output_dir, exist_ok=True)
    root_files = [os.path.join(args.input_dir, f) for f in os.listdir(args.input_dir) if f.endswith('.root')]

    # Filter files
    if args.checkFiles:
        root_files = check_files(root_files, args.gridSize, 'photoSensor_hits_histograms')
    if args.doCuts:
        root_files = apply_cuts(root_files, args.minNHits, args.minPrimarySteps,
                                'photoSensor_hits_histograms', 'primary;1')

    # DataFrame generation
    df_hits_paths = process_hits_df(root_files, 'photoSensor_hits_histograms',
                                    os.path.join(args.output_dir, 'DF_hits'),
                                    args.overwriteDF, args.useHistograms) if args.saveDFhits else []
    df_pri_paths  = process_primary_df(root_files, 'primary;1',
                                       os.path.join(args.output_dir, 'DF_primary'),
                                       args.overwriteDF) if args.saveDFprimary else []

    # Prepare grid output paths
    gridSize = tuple(args.gridSize)
    base_hits_npy = os.path.join(args.output_dir, 'grid_hits')
    base_pri_npy  = os.path.join(args.output_dir, 'grid_primary')
    base_h5       = os.path.join(args.output_dir, 'grid_h5')
    paths_npy_hits  = [f"{base_hits_npy}_{i}.npy" for i in range(len(df_hits_paths))]
    paths_npy_pri   = [f"{base_pri_npy}_{i}.npy" for i in range(len(df_pri_paths))]
    paths_h5        = [f"{base_h5}_{i}.h5" for i in range(len(df_hits_paths))]

    # Grid creation
    if args.saveGridNpyHits or args.saveGridNpyPrimary or args.saveGridH5:
        create_grids(
            df_hits_paths, df_pri_paths,
            paths_npy_hits, paths_npy_pri, paths_h5,
            args.saveGridNpyHits, args.saveGridNpyPrimary, args.saveGridH5,
            gridSize,
            combine_errors=False,
            combine_walls=True,
            walls_combine=False,
            walls_method=expNWalls,
            combine_vectors=True
        )

    # Data splitting
    if args.nTest > 0 or args.nVal > 0:
        train_paths, test_paths, val_paths = split_data(paths_h5, args.nTest, args.nVal, args.seed)
        # Save lists
        for name, lst in [('train', train_paths), ('test', test_paths), ('val', val_paths)]:
            with open(os.path.join(args.output_dir, f"{name}_paths.txt"), 'w') as f:
                f.write("\n".join(lst))

    # UNet training
    if args.train:
        checkpoint_dir = args.checkpointDir or os.path.join(args.output_dir, 'checkpoints')
        os.makedirs(checkpoint_dir, exist_ok=True)
        config_path = os.path.join(args.output_dir, 'config.yml')
        cfg = get_config(train_paths, val_paths, checkpoint_dir, num_workers=args.numWorkers)
        save_config(cfg, config_path)
        train_main(['--config', config_path])


if __name__ == '__main__':
    main()
