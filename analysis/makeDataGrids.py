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

# allow imports from parent python directory
sys.path.append('../python/')
from constants import CM_PER_RAD, MM_PER_CM, Y_LIM, DETECTOR_SIZE_MM
from importMethods import (
    get_histogram_nHits_total,
    get_histogram_hits_tuple,
    get_photosensor_hits_photosensor_ID,
    get_photosensor_hits_photosensor_direction,
    get_photosensor_hits_photosensor_position,
    get_photosensor_hits_photosensor_wall,
    get_photosensor_hits_position_relative_binned,
    get_photosensor_hits_position_relative_nBin,
    get_photosensor_hits_position_initial,
    get_primary_position,
    get_primary_pdg
)
from hitAccuracyMethods import make_r, make_theta, make_phi, make_reconstructedVector_direction, make_relativeVector
from filterMethods import filter_r
from gridMethods import get_voxelGrid, make_voxelGrid_truth, expNWalls, wallStringToInt

# Global logger
LOGGER = logging.getLogger('makeDataGrids')

def configure_logging(verbosity: str):
    level = getattr(logging, verbosity.upper(), None)
    if not isinstance(level, int):
        raise ValueError(f"Invalid verbosity level: {verbosity}")
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(name)s - %(filename)s::%(funcName)s(%(lineno)d) - %(levelname)s - %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S'
    )
    logging.getLogger("fsspec.local").setLevel(logging.WARNING)


def r_to_theta(r):
    return r / (CM_PER_RAD * MM_PER_CM)


def check_files(paths, hist_dir, num_workers=1):
    """
    Verify that each ROOT file contains histograms of the same shape.
    """
    expected = None
    valid = []
    for path in paths:
        try:
            LOGGER.debug(f"Opening file: {path}")
            with uproot.open(path, num_workers=num_workers) as f:
                hists = f[hist_dir]
                for key in hists.keys():
                    arr = hists[key].values()
                    if expected is None:
                        expected = arr.shape
                        LOGGER.debug(f"Expected histogram shape set to {expected}")
                    if arr.shape != expected:
                        LOGGER.error(f"{key} has shape {arr.shape}, expected {expected}")
                        raise ValueError(f"Shape mismatch for {key}")
                valid.append(path)
        except Exception as e:
            LOGGER.warning(f"Skipping file {path}: {e}")
    LOGGER.info(f"Valid files after check: {len(valid)}/{len(paths)}")
    return valid


def apply_cuts(paths, min_hits, min_primary_steps, hist_dir, primary_tree):
    """
    Filter files by total hit count and number of primary steps.
    """
    filtered = []
    for path in paths:
        try:
            n_hits = get_histogram_nHits_total(path, directoryName=hist_dir)
            prim_positions = get_primary_position(path, primary_tree)
            LOGGER.debug(f"{path}: n_hits={n_hits}, primary_steps={len(prim_positions)}")
            if n_hits >= min_hits and len(prim_positions) >= min_primary_steps:
                filtered.append(path)
            else:
                LOGGER.info(f"File {path} excluded by cuts: hits={n_hits}, steps={len(prim_positions)}")
        except Exception as e:
            LOGGER.error(f"Error applying cuts to {path}: {e}")
    LOGGER.info(f"Files after cuts: {len(filtered)}/{len(paths)}")
    return filtered


def build_hits_dataframes(paths, hist_dir, use_histograms):
    """
    Build hits DataFrames in-memory for each file (without saving).
    """
    dfs = []
    for path in tqdm(paths, desc="Building hits DataFrames"):
        try:
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
                    'sensor_name': get_photosensor_hits_photosensor_ID(path, 'photoSensor_hits;1', hist_dir, verbose=False),
                    'sensor_direction': get_photosensor_hits_photosensor_direction(path, 'photoSensor_hits;1', hist_dir, verbose=False),
                    'sensor_position': get_photosensor_hits_photosensor_position(path, 'photoSensor_hits;1', hist_dir, verbose=False),
                    'sensor_wall': get_photosensor_hits_photosensor_wall(path, 'photoSensor_hits;1', hist_dir, verbose=False),
                    'relativePosition_binned': get_photosensor_hits_position_relative_binned(path, 'photoSensor_hits;1', hist_dir, verbose=False),
                    'relativePosition_nBin': get_photosensor_hits_position_relative_nBin(path, 'photoSensor_hits;1', hist_dir, verbose=False),
                    'initialPosition': get_photosensor_hits_position_initial(path, 'photoSensor_hits;1', verbose=False)
                })
            # Feature engineering
            df = make_r(df)
            df = filter_r(df, Y_LIM)
            df = make_theta(df, r_to_theta)
            df = make_phi(df)
            df = make_reconstructedVector_direction(df)
            if 'initialPosition' in df.columns:
                df = make_relativeVector(df)
            dfs.append(df)
        except Exception as e:
            LOGGER.error(f"Failed to build hits DF for {path}: {e}")
    return dfs


def save_hits_dataframes(dfs, output_base, overwrite):
    """
    Save hits DataFrames to parquet files if desired.
    """
    for i, df in enumerate(dfs):
        out_path = f"{output_base}_{i}.parquet"
        if overwrite or not os.path.exists(out_path):
            df.to_parquet(out_path, compression='snappy')
            LOGGER.info(f"Saved hits DataFrame: {out_path}")
        else:
            LOGGER.info(f"Skipping existing DataFrame: {out_path}")


def build_primary_dataframes(paths, primary_tree, pdg_code):
    """
    Build primary particle DataFrames in-memory for each file (without saving).
    """
    dfs = []
    half = np.array(DETECTOR_SIZE_MM) / 2
    for path in tqdm(paths, desc="Building primary DataFrames"):
        try:
            positions = get_primary_position(path, primary_tree)
            pdgs = get_primary_pdg(path, primary_tree)
            df = pd.DataFrame({'position': positions, 'pdg': pdgs})
            df = df[df['pdg'] == pdg_code]
            df = df[df['position'].apply(lambda xyz: all(-half[j] < xyz[j] < half[j] for j in range(3)))]
            dfs.append(df)
        except Exception as e:
            LOGGER.error(f"Failed to build primary DF for {path}: {e}")
    return dfs


def save_primary_dataframes(dfs, output_base, overwrite):
    """
    Save primary DataFrames to parquet files if desired.
    """
    for i, df in enumerate(dfs):
        out_path = f"{output_base}_{i}.parquet"
        if overwrite or not os.path.exists(out_path):
            df.to_parquet(out_path, compression='snappy')
            LOGGER.info(f"Saved primary DataFrame: {out_path}")
        else:
            LOGGER.info(f"Skipping existing DataFrame: {out_path}")


def create_grids(dfs_hits, dfs_primary, paths_npy_hits, paths_npy_primary, paths_h5,
                 save_npy_hits, save_npy_primary, save_h5,
                 grid_shape, combine_errors, combine_walls, walls_combine, walls_method,
                 combine_vectors, num_workers):
    """
    Create voxel grids and save as .npy and/or .h5 using Parallel processing.
    """
    LOGGER.info("Starting voxel grid creation")

    def _create_one(args):
        dfh, dfp, nh, npf, h5p = args
        do_npy_hit = save_npy_hits and not os.path.exists(nh)
        do_npy_pri = save_npy_primary and not os.path.exists(npf)
        do_h5 = save_h5 and not os.path.exists(h5p)
        if not (do_npy_hit or do_npy_pri or do_h5):
            LOGGER.debug(f"Skipping grid creation for {h5p}")
            return
        try:
            x, y = None, None
            if save_npy_hits or save_h5:
                starts = np.vstack(dfh['sensor_position'].tolist()).reshape(-1, 3)
                dirs = -np.vstack(dfh['reconstructedVector_direction'].tolist()).reshape(-1, 3)
                x = get_voxelGrid(
                    grid_shape=np.array(grid_shape, int),
                    grid_dimensions=np.array(DETECTOR_SIZE_MM),
                    vector_starts=starts,
                    vector_directions=dirs,
                    vector_weights=None,
                    vector_start_walls=wallStringToInt(dfh['sensor_wall'].to_numpy().astype(str)),
                    walls=combine_walls,
                    walls_combine=walls_combine,
                    walls_combine_method=walls_method,
                    vector_combine=combine_vectors
                )
            if save_npy_primary or save_h5:
                positions = np.vstack(dfp['position'].tolist())
                y = make_voxelGrid_truth(
                    positions,
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
                LOGGER.info(f"Saved H5 grid: {h5p}")
            if do_npy_hit and x is not None:
                np.save(nh, x)
                LOGGER.debug(f"Saved NPY hit grid: {nh}")
            if do_npy_pri and y is not None:
                np.save(npf, y)
                LOGGER.debug(f"Saved NPY primary grid: {npf}")
        except Exception as e:
            LOGGER.error(f"Error in grid creation for {h5p}: {e}")

    jobs = list(zip(dfs_hits, dfs_primary, paths_npy_hits, paths_npy_primary, paths_h5))
    Parallel(n_jobs=num_workers)(delayed(_create_one)(job) for job in tqdm(jobs, desc="Creating voxel grids"))


def split_data(all_h5_paths, n_test, n_val, seed):
    """
    Split dataset into train/test/val lists.
    """
    LOGGER.info(f"Splitting {len(all_h5_paths)} files into train/test/val")
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
    LOGGER.info(f"Train: {len(train)}, Test: {len(test)}, Val: {len(val)}")
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
    LOGGER.info("Starting data grid generation")

    # prepare output
    os.makedirs(args.output_dir, exist_ok=True)
    root_files = [os.path.join(args.input_dir, f) for f in os.listdir(args.input_dir) if f.endswith('.root')]
    LOGGER.info(f"Found {len(root_files)} root files in {args.input_dir}")

    # File checks and cuts
    if not args.noCheckFiles:
        root_files = check_files(root_files, 'photoSensor_hits_histograms', num_workers=args.numWorkers)
    if not args.noCuts:
        root_files = apply_cuts(root_files, args.minNHits, args.minPrimarySteps,
                                'photoSensor_hits_histograms', 'primary;1')

    # Build DataFrames always if grids or saving requested
    dfs_hits, dfs_primary = [], []
    need_hits = args.saveDFhits or args.saveGridNpyHits or args.saveGridH5
    need_pri  = args.saveDFprimary or args.saveGridNpyPrimary or args.saveGridH5

    if need_hits:
        LOGGER.info("Building hits DataFrames in memory")
        dfs_hits = build_hits_dataframes(root_files, 'photoSensor_hits_histograms', args.useHistograms)
        if args.saveDFhits:
            save_hits_dataframes(dfs_hits, os.path.join(args.output_dir, 'DF_hits'), args.overwriteDF)
        else:
            LOGGER.debug("Skipping saving hits DataFrames to disk")

    if need_pri:
        LOGGER.info("Building primary DataFrames in memory")
        dfs_primary = build_primary_dataframes(root_files, 'primary;1', args.primaryPdg)
        if args.saveDFprimary:
            save_primary_dataframes(dfs_primary, os.path.join(args.output_dir, 'DF_primary'), args.overwriteDF)
        else:
            LOGGER.debug("Skipping saving primary DataFrames to disk")

    # Prepare grid output paths
    grid_shape = tuple(args.gridSize)
    base_hits_npy = os.path.join(args.output_dir, 'grid_hits')
    base_pri_npy  = os.path.join(args.output_dir, 'grid_primary')
    base_h5       = os.path.join(args.output_dir, 'grid_h5')

    paths_npy_hits     = [f"{base_hits_npy}_{i}.npy" for i in range(len(dfs_hits))]
    paths_npy_primary  = [f"{base_pri_npy}_{i}.npy" for i in range(len(dfs_primary))]
    paths_h5           = [f"{base_h5}_{i}.h5" for i in range(len(dfs_hits))]

    # Grid creation
    if args.saveGridNpyHits or args.saveGridNpyPrimary or args.saveGridH5:
        create_grids(
            dfs_hits, dfs_primary,
            paths_npy_hits, paths_npy_primary, paths_h5,
            args.saveGridNpyHits, args.saveGridNpyPrimary, args.saveGridH5,
            grid_shape,
            combine_errors=False,
            combine_walls=True,
            walls_combine=False,
            walls_method=expNWalls,
            combine_vectors=True,
            num_workers=args.numWorkers
        )

    # Data splitting
    if not args.noSplit and args.saveGridH5:
        train_paths, test_paths, val_paths = split_data(paths_h5, args.nTest, args.nVal, seed=42)
        for name, lst in [('train', train_paths), ('test', test_paths), ('val', val_paths)]:
            out_file = os.path.join(args.output_dir, f"{name}_paths.txt")
            with open(out_file, 'w') as f:
                f.write("\n".join(lst))
            LOGGER.info(f"Wrote {name} paths to {out_file}")

    LOGGER.info("Data grid generation completed.")

if __name__ == '__main__':
    main()
