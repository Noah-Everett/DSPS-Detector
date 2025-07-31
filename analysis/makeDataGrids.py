#!/usr/bin/env python3
"""
makeDataGrids.py

Script to process ROOT files, generate DataFrames and voxel grids for ML,
and split data into train/val/test.
"""
import argparse
import logging
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

# Import all methods (unknown source mapping)
from filterMethods import *
from timeMethods import *
from gridMethods import *
from trackReconstructionMethods import *
from vertexReconstructionMethods import *
from hitAccuracyMethods import *
from statisticsMethods import *
from importMethods import *
from constants import *
from plotMethods import *
from UNetMethods import *

def configure_logging(verbosity: str):
    level = getattr(logging, verbosity.upper(), None)
    if not isinstance(level, int):
        raise ValueError(f"Invalid verbosity level: {verbosity}")
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S'
    )
    logging.debug(f"Logging configured to {verbosity.upper()} level.")


def r_to_theta(r):
    return r / (CM_PER_RAD * MM_PER_CM)


def check_files(paths, hist_dir):
    """
    Verify that each ROOT file contains histograms of the same shape.
    """
    logger = logging.getLogger('check_files')
    expected = None
    valid = []
    for path in paths:
        try:
            logger.debug(f"Opening file: {path}")
            f = uproot.open(path)
            hists = f[hist_dir]
            for key in hists.keys():
                arr = hists[key].values()
                if expected is None:
                    expected = arr.shape
                    logger.debug(f"Expected histogram shape set to {expected}")
                if arr.shape != expected:
                    logger.error(f"{key} has shape {arr.shape}, expected {expected}")
                    raise ValueError(f"Shape mismatch for {key}")
            valid.append(path)
        except Exception as e:
            logger.warning(f"Skipping file {path}: {e}")
    logger.info(f"Valid files after check: {len(valid)}/{len(paths)}")
    return valid


def apply_cuts(paths, min_hits, min_primary_steps, hist_dir, primary_tree):
    """
    Filter files by total hit count and number of primary steps.
    """
    logger = logging.getLogger('apply_cuts')
    filtered = []
    for path in paths:
        try:
            n_hits = get_histogram_nHits_total(path, directoryName=hist_dir)
            sizes = get_histogram_sizes(path, directoryName=hist_dir)
            prim_positions = get_primary_position(path, primary_tree)
            logger.debug(f"{path}: n_hits={n_hits}, primary_steps={len(prim_positions)}")
            if n_hits >= min_hits and len(prim_positions) >= min_primary_steps:
                filtered.append(path)
            else:
                logger.info(f"File {path} excluded by cuts: hits={n_hits}, steps={len(prim_positions)}")
        except Exception as e:
            logger.error(f"Error applying cuts to {path}: {e}")
    logger.info(f"Files after cuts: {len(filtered)}/{len(paths)}")
    return filtered


def process_hits_df(paths, hist_dir, output_base, overwrite, use_histograms):
    """
    Build and save hits DataFrame for each file.
    """
    logger = logging.getLogger('process_hits_df')
    logger.info(f"Processing {len(paths)} files for hits DataFrames")
    df_paths = []
    for i, path in enumerate(tqdm(paths, desc="Processing hits DataFrames")):
        out_path = f"{output_base}_{i}.parquet"
        logger.debug(f"Output path: {out_path}")
        if not overwrite and os.path.exists(out_path):
            logger.info(f"Skipping existing DataFrame: {out_path}")
            df_paths.append(out_path)
            continue
        try:
            if use_histograms:
                ids, dirs, poss, walls, rel_binned, rel_nbin = get_histogram_hits_tuple(path, hist_dir, True)
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
                    'initialPosition': get_photosensor_initialPosition(path, hist_dir)
                })
            # Feature engineering
            df = make_r(df)
            df = filter_r(df, Y_LIM)
            df = make_theta(df, r_to_theta)
            df = make_phi(df)
            df = make_reconstructedVector_direction(df)
            if 'initialPosition' in df.columns:
                df = make_relativeVector(df)
            df.to_parquet(out_path, compression='snappy')
            logger.info(f"Saved hits DataFrame: {out_path}")
            df_paths.append(out_path)
        except Exception as e:
            logger.error(f"Failed to process hits for {path}: {e}")
    return df_paths


def process_primary_df(paths, primary_tree, output_base, overwrite, pdg_code):
    """
    Build and save primary DataFrame for each file, filtering by pdg code.
    """
    logger = logging.getLogger('process_primary_df')
    logger.info(f"Processing {len(paths)} files for primary DataFrames")
    df_paths = []
    for i, path in enumerate(tqdm(paths, desc="Processing primary DataFrames")):
        out_path = f"{output_base}_{i}.parquet"
        logger.debug(f"Output path: {out_path}")
        if not overwrite and os.path.exists(out_path):
            logger.info(f"Skipping existing DataFrame: {out_path}")
            df_paths.append(out_path)
            continue
        try:
            positions = get_primary_position(path, primary_tree)
            pdgs = get_primary_pdg(path, primary_tree)
            df = pd.DataFrame({'position': positions, 'pdg': pdgs})
            # Filter by user-specified pdg and detector bounds
            df = df[df['pdg'] == pdg_code]
            half = np.array(DETECTOR_SIZE_MM) / 2
            df = df[df['position'].apply(lambda xyz: all(-half[j] < xyz[j] < half[j] for j in range(3)))]
            df.to_parquet(out_path, compression='snappy')
            logger.info(f"Saved primary DataFrame: {out_path}")
            df_paths.append(out_path)
        except Exception as e:
            logger.error(f"Failed to process primary for {path}: {e}")
    return df_paths


def create_grids(df_paths_hits, df_paths_primary, paths_npy_hits, paths_npy_primary, paths_h5,
                 save_npy_hits, save_npy_primary, save_h5,
                 grid_shape, combine_errors, combine_walls, walls_combine, walls_method, combine_vectors,
                 num_workers):
    """
    Create voxel grids and save as .npy and/or .h5 using Parallel.
    """
    logger = logging.getLogger('create_grids')
    logger.info("Starting voxel grid creation")
    def _create_one(args):
        dfh, dfp, nh, npf, h5p = args
        do_npy_hit = save_npy_hits and not os.path.exists(nh)
        do_npy_pri = save_npy_primary and not os.path.exists(npf)
        do_h5 = save_h5 and not os.path.exists(h5p)
        if not (do_npy_hit or do_npy_pri or do_h5):
            logger.debug(f"Skipping grid creation for {h5p}")
            return
        try:
            x = None
            if save_npy_hits or save_h5:
                dfh_df = pd.read_parquet(dfh)
                x = get_voxelGrid(
                    grid_shape=np.array(grid_shape, int),
                    grid_dimensions=np.array(DETECTOR_SIZE_MM),
                    vector_starts=np.array(dfh_df['sensor_position'].tolist()).reshape(-1, 3),
                    vector_directions=-np.array(dfh_df['reconstructedVector_direction'].tolist()).reshape(-1, 3),
                    vector_weights=None,
                    make_errors=combine_errors,
                    make_walls=combine_walls,
                    make_walls_combine=walls_combine,
                    walls_combine_method=walls_method,
                    combine_vectors=combine_vectors
                )
            y = None
            if save_npy_primary or save_h5:
                dfp_df = pd.read_parquet(dfp)
                y = make_voxelGrid_truth(
                    np.array(dfp_df['position'].tolist()),
                    shape=grid_shape,
                    detectorDimensions=DETECTOR_SIZE_MM,
                    makeErrors=False
                )[0]
            if do_h5:
                with h5py.File(h5p, 'w') as f:
                    if x is not None:
                        f.create_dataset('x', data=np.moveaxis(x, -1, 0))
                    if y is not None:
                        f.create_dataset('y', data=y)
                logger.info(f"Saved H5 grid: {h5p}")
            if do_npy_hit and x is not None:
                np.save(nh, x)
                logger.debug(f"Saved NPY hit grid: {nh}")
            if do_npy_pri and y is not None:
                np.save(npf, y)
                logger.debug(f"Saved NPY primary grid: {npf}")
        except Exception as e:
            logger.error(f"Error in grid creation for args {args}: {e}")

    jobs = list(zip(df_paths_hits, df_paths_primary, paths_npy_hits, paths_npy_primary, paths_h5))
    Parallel(n_jobs=num_workers)(delayed(_create_one)(job) for job in tqdm(jobs, desc="Creating voxel grids"))


def split_data(all_h5_paths, n_test, n_val, seed):
    """
    Split dataset into train/test/val lists.
    """
    logger = logging.getLogger('split_data')
    logger.info(f"Splitting {len(all_h5_paths)} files into train/test/val")
    np.random.seed(seed)
    total = len(all_h5_paths)
    indices = np.arange(total)
    test_idx = np.random.choice(indices, size=n_test, replace=False)
    remaining = np.setdiff1d(indices, test_idx)
    val_idx = np.random.choice(remaining, size=n_val, replace=False)
    train_idx = np.setdiff1d(remaining, val_idx)
    train = [all_h5_paths[i] for i in train_idx]
    test = [all_h5_paths[i] for i in test_idx]
    val = [all_h5_paths[i] for i in val_idx]
    logger.info(f"Train: {len(train)}, Test: {len(test)}, Val: {len(val)}")
    return train, test, val


def main():
    parser = argparse.ArgumentParser(description='Generate ML data grids with logging')
    parser.add_argument('input_dir', help='Directory of input ROOT files')
    parser.add_argument('output_dir', help='Base output directory')
    parser.add_argument('--gridSize', type=int, nargs=3, default=[80,80,80],
                        help='Voxel grid size (default: 80 80 80)')
    parser.add_argument('--minNHits', type=int, default=0,
                        help='Minimum number of hits (default: 0)')
    parser.add_argument('--minPrimarySteps', type=int, default=30,
                        help='Minimum number of primary steps (default: 30)')
    parser.add_argument('--primaryPdg', type=int, default=13,
                        help='PDG code for primary particles (default: 13)')
    parser.add_argument('--noCuts', action='store_true',
                        help='Skip applying hit/step cuts (default: apply cuts)')
    parser.add_argument('--noCheckFiles', action='store_true',
                        help='Skip verifying histogram shapes (default: check files)')
    parser.add_argument('--useHistograms', action='store_true',
                        help='Use histogram-based hit tuple')
    parser.add_argument('--overwriteDF', action='store_true',
                        help='Overwrite existing DataFrames')
    parser.add_argument('--saveDFhits', action='store_true',
                        help='Save hits DataFrames')
    parser.add_argument('--saveDFprimary', action='store_true',
                        help='Save primary DataFrames')
    parser.add_argument('--saveGridNpyHits', action='store_true',
                        help='Save hit grids as .npy')
    parser.add_argument('--saveGridNpyPrimary', action='store_true',
                        help='Save primary grids as .npy')
    parser.add_argument('--saveGridH5', action='store_true',
                        help='Save grids as .h5')
    parser.add_argument('--numWorkers', type=int, default=1,
                        help='Number of parallel workers (default: 1)')
    parser.add_argument('--nTest', type=int, default=20,
                        help='Number of test examples (default: 20)')
    parser.add_argument('--nVal', type=int, default=10,
                        help='Number of validation examples (default: 10)')
    parser.add_argument('--noSplit', action='store_true',
                        help='Do not split data into train/test/val (default: split)')
    parser.add_argument('--verbosity', '-v', choices=['debug','info','warning','error','critical'], default='info',
                        help='Logging verbosity level (default: info)')
    args = parser.parse_args()

    configure_logging(args.verbosity)
    logger = logging.getLogger('main')

    logger.info("Starting data grid generation")
    os.makedirs(args.output_dir, exist_ok=True)
    root_files = [os.path.join(args.input_dir, f) for f in os.listdir(args.input_dir) if f.endswith('.root')]
    logger.info(f"Found {len(root_files)} root files in {args.input_dir}")

    # File checks and cuts
    if not args.noCheckFiles:
        root_files = check_files(root_files, 'photoSensor_hits_histograms')
    if not args.noCuts:
        root_files = apply_cuts(root_files, args.minNHits, args.minPrimarySteps,
                                'photoSensor_hits_histograms', 'primary;1')

    # DataFrame generation
    df_hits_paths = []
    if args.saveDFhits:
        df_hits_paths = process_hits_df(root_files, 'photoSensor_hits_histograms',
                                        os.path.join(args.output_dir, 'DF_hits'),
                                        args.overwriteDF, args.useHistograms)
    df_pri_paths = []
    if args.saveDFprimary:
        df_pri_paths = process_primary_df(root_files, 'primary;1',
                                          os.path.join(args.output_dir, 'DF_primary'),
                                          args.overwriteDF, args.primaryPdg)

    # Prepare grid output paths
    gridSize = tuple(args.gridSize)
    base_hits_npy = os.path.join(args.output_dir, 'grid_hits')
    base_pri_npy = os.path.join(args.output_dir, 'grid_primary')
    base_h5 = os.path.join(args.output_dir, 'grid_h5')
    paths_npy_hits = [f"{base_hits_npy}_{i}.npy" for i in range(len(df_hits_paths))]
    paths_npy_pri = [f"{base_pri_npy}_{i}.npy" for i in range(len(df_pri_paths))]
    paths_h5 = [f"{base_h5}_{i}.h5" for i in range(len(df_hits_paths))]

    # Grid creation
    if args.saveGridNpyHits or args.saveGridNpyPrimary or args.saveGridH5:
        create_grids(df_hits_paths, df_pri_paths,
                     paths_npy_hits, paths_npy_pri, paths_h5,
                     args.saveGridNpyHits, args.saveGridNpyPrimary, args.saveGridH5,
                     gridSize,
                     combine_errors=False,
                     combine_walls=True,
                     walls_combine=False,
                     walls_method=expNWalls,
                     combine_vectors=True,
                     num_workers=args.numWorkers)

    # Data splitting
    if not args.noSplit:
        train_paths, test_paths, val_paths = split_data(paths_h5, args.nTest, args.nVal, seed=42)
        for name, lst in [('train', train_paths), ('test', test_paths), ('val', val_paths)]:
            out_file = os.path.join(args.output_dir, f"{name}_paths.txt")
            with open(out_file, 'w') as f:
                f.write("\n".join(lst))
            logger.info(f"Wrote {name} paths to {out_file}")

    logger.info("Data grid generation completed.")

if __name__ == '__main__':
    main()
