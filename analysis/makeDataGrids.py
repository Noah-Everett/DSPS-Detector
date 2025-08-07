#!/usr/bin/env python3
"""
makeDataGrids.py

Script to process ROOT files, generate DataFrames and voxel grids for ML,
and split data into train/val/test. All steps are fully parallelized using joblib.
All intermediate DataFrames are saved on disk (or in a system temp directory) and
cleaned up if not retained long-term to manage memory use.
Supports limiting the number of input files with `-n/--nFiles`.
"""
import argparse
import logging
import os
import sys
import gc
import tempfile
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
from hitAccuracyMethods import (
    make_r, make_theta, make_phi,
    make_reconstructedVector_direction, make_relativeVector
)
from filterMethods import filter_r
from gridMethods import get_voxelGrid, make_voxelGrid_truth, expNWalls, wallStringToInt

# Global logger
LOGGER = logging.getLogger('makeDataGrids')


def configure_logging(verbosity: str):
    """
    Configure root logger formatting and level.
    """
    level = getattr(logging, verbosity.upper(), None)
    if not isinstance(level, int):
        raise ValueError(f"Invalid verbosity level: {verbosity}")
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(name)s - %(filename)s::%(funcName)s(%(lineno)d) - %(levelname)s - %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S'
    )
    logging.getLogger("fsspec.local").setLevel(logging.WARNING)
    logging.getLogger("numba.core.byteflow").setLevel(logging.WARNING)
    logging.getLogger("numba.core.ssa").setLevel(logging.WARNING)
    logging.getLogger("numba.core.interpreter").setLevel(logging.WARNING)
    logging.getLogger("numba.core.entrypoints").setLevel(logging.WARNING)


def r_to_theta(r):
    """
    Convert radius to theta using constants.
    """
    return r / (CM_PER_RAD * MM_PER_CM)


def check_files(paths, hist_dir, num_workers):
    """
    Verify that each ROOT file contains histograms of the same shape,
    based on the first file's first histogram.
    Parallelized over files.
    """
    if not paths:
        return []
    # Infer expected shape
    try:
        print('paths[0]:', paths[0])
        with uproot.open(paths[0], num_workers=num_workers) as f0:
            h0 = f0[hist_dir]
            first_key = next(iter(h0.keys()))
            expected = h0[first_key].values().shape
            LOGGER.info(f"Expected histogram shape: {expected}")
    except Exception as e:
        LOGGER.error(f"Failed to infer expected shape from {paths[0]}: {e}")
        return []

    def _check(path):
        try:
            with uproot.open(path, num_workers=num_workers) as f:
                h = f[hist_dir]
                for key in h.keys():
                    arr = h[key].values()
                    if arr.shape != expected:
                        LOGGER.error(f"{path}:{key} shape {arr.shape} != expected {expected}")
                        return None
            return path
        except Exception as e:
            LOGGER.warning(f"Skipping file {path}: {e}")
            return None

    LOGGER.info(f"Checking {len(paths)} files for consistent histogram shapes...")
    valid = Parallel(n_jobs=num_workers, backend='threading')(
        delayed(_check)(p) for p in tqdm(paths, desc="Checking files")
    )
    valid = [p for p in valid if p]
    LOGGER.info(f"Valid files after check: {len(valid)}/{len(paths)}")
    return valid


def apply_cuts(paths, min_hits, min_primary_steps, hist_dir, primary_tree, num_workers):
    """
    Filter files by total hit count and number of primary steps, in parallel.
    """
    def _cut(path):
        try:
            n_hits = get_histogram_nHits_total(path, directoryName=hist_dir)
            prim_steps = len(get_primary_position(path, primary_tree))
            if n_hits >= min_hits and prim_steps >= min_primary_steps:
                return path
            LOGGER.info(f"Excluding {path}: hits={n_hits}, steps={prim_steps}")
        except Exception as e:
            LOGGER.error(f"Error applying cuts to {path}: {e}")
        return None

    LOGGER.info(f"Applying cuts (min_hits={min_hits}, min_steps={min_primary_steps})...")
    filtered = Parallel(n_jobs=num_workers)(
        delayed(_cut)(p) for p in tqdm(paths, desc="Applying cuts")
    )
    filtered = [p for p in filtered if p]
    LOGGER.info(f"Files after cuts: {len(filtered)}/{len(paths)}")
    return filtered


def process_hits_df(paths, hist_dir, output_base, overwrite, use_histograms, num_workers):
    """
    Parallel build & save hits DataFrames; returns list of parquet paths.
    """
    LOGGER.info(f"Processing {len(paths)} hits DataFrames...")

    def _build(i, path):
        out_path = f"{output_base}_{i}.parquet"
        if not overwrite and os.path.exists(out_path):
            LOGGER.debug(f"Skipping existing: {out_path}")
            return out_path
        if use_histograms:
            ids, dirs, poss, walls, rb, rn = get_histogram_hits_tuple(path, hist_dir, True)
            df = pd.DataFrame({
                'sensor_name': ids,
                'sensor_direction': dirs,
                'sensor_position': poss,
                'sensor_wall': walls,
                'relativePosition_binned': rb,
                'relativePosition_nBin': rn,
            })
        else:
            df = pd.DataFrame({
                'sensor_name': get_photosensor_hits_photosensor_ID(path, 'photoSensor_hits;1', hist_dir),
                'sensor_direction': get_photosensor_hits_photosensor_direction(path, 'photoSensor_hits;1', hist_dir),
                'sensor_position': get_photosensor_hits_photosensor_position(path, 'photoSensor_hits;1', hist_dir),
                'sensor_wall': get_photosensor_hits_photosensor_wall(path, 'photoSensor_hits;1', hist_dir),
                'relativePosition_binned': get_photosensor_hits_position_relative_binned(path, 'photoSensor_hits;1', hist_dir),
                'relativePosition_nBin': get_photosensor_hits_position_relative_nBin(path, 'photoSensor_hits;1', hist_dir),
                'initialPosition': get_photosensor_hits_position_initial(path, 'photoSensor_hits;1')
            })
        df = make_r(df)
        df = filter_r(df, Y_LIM)
        df = make_theta(df, r_to_theta)
        df = make_phi(df)
        df = make_reconstructedVector_direction(df)
        if 'initialPosition' in df.columns:
            df = make_relativeVector(df)
        df.to_parquet(out_path, compression='snappy')
        LOGGER.debug(f"Saved hits DF: {out_path}")
        del df
        gc.collect()
        return out_path

    paths_out = Parallel(n_jobs=num_workers)(
        delayed(_build)(i, p) for i, p in enumerate(tqdm(paths, desc="Building hits DFs"))
    )
    return paths_out


def process_primary_df(paths, primary_tree, output_base, overwrite, pdg_code, num_workers):
    """
    Parallel build & save primary DataFrames; returns list of parquet paths.
    """
    LOGGER.info(f"Processing {len(paths)} primary DataFrames...")

    def _build(i, path):
        out_path = f"{output_base}_{i}.parquet"
        if not overwrite and os.path.exists(out_path):
            LOGGER.debug(f"Skipping existing: {out_path}")
            return out_path
        positions = get_primary_position(path, primary_tree)
        pdgs = get_primary_pdg(path, primary_tree)
        df = pd.DataFrame({'position': positions, 'pdg': pdgs})
        half = np.array(DETECTOR_SIZE_MM) / 2
        df = df[df['pdg'] == pdg_code]
        df = df[df['position'].apply(lambda xyz: all(-half[j] < xyz[j] < half[j] for j in range(3)))]
        df.to_parquet(out_path, compression='snappy')
        LOGGER.debug(f"Saved primary DF: {out_path}")
        del df, positions, pdgs
        gc.collect()
        return out_path

    paths_out = Parallel(n_jobs=num_workers)(
        delayed(_build)(i, p) for i, p in enumerate(tqdm(paths, desc="Building primary DFs"))
    )
    return paths_out


def create_grids(df_hits, df_pri,
                 npy_hits, npy_pri, h5_paths,
                 save_npy_hits, save_npy_pri, save_h5,
                 grid_shape, combine_errors, combine_walls,
                 walls_combine, walls_method, combine_vectors,
                 num_workers):
    """
    Parallel create voxel grids and save as .npy/.h5.
    """
    LOGGER.info("Creating voxel grids...")
    jobs = list(zip(df_hits, df_pri, npy_hits, npy_pri, h5_paths))

    def _make(idx, dfh_p, dfp_p, nh, npf, h5p):
        do_npy_h = save_npy_hits and not os.path.exists(nh)
        do_npy_p = save_npy_pri and not os.path.exists(npf)
        do_h5    = save_h5 and not os.path.exists(h5p)
        if not (do_npy_h or do_npy_p or do_h5):
            return
        x = y = None
        if save_npy_hits or save_h5:
            dfh = pd.read_parquet(dfh_p)
            starts = np.vstack(dfh['sensor_position'].tolist())
            dirs = -np.vstack(dfh['reconstructedVector_direction'].tolist())
            x = get_voxelGrid(
                grid_shape=np.array(grid_shape),
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
            del dfh, starts, dirs
            gc.collect()
        if save_npy_pri or save_h5:
            dfp = pd.read_parquet(dfp_p)
            pos = np.vstack(dfp['position'].tolist())
            y = make_voxelGrid_truth(pos, shape=grid_shape,
                                     detectorDimensions=DETECTOR_SIZE_MM,
                                     makeErrors=False)[0]
            del dfp, pos
            gc.collect()
        if do_h5:
            with h5py.File(h5p, 'w') as f:
                if x is not None: f.create_dataset('x', data=np.moveaxis(x, -1, 0))
                if y is not None: f.create_dataset('y', data=y)
            LOGGER.debug(f"Saved H5: {h5p}")
        if do_npy_h and x is not None:
            np.save(nh, x)
        if do_npy_p and y is not None:
            np.save(npf, y)
        del x, y
        gc.collect()

    Parallel(n_jobs=num_workers)(
        delayed(_make)(i, *job) for i, job in enumerate(tqdm(jobs, desc="Creating voxel grids"))
    )


def split_data(all_h5, n_test, n_val, seed):
    """
    Split dataset into train/test/val lists.
    """
    LOGGER.info(f"Splitting {len(all_h5)} files into train/test/val")
    np.random.seed(seed)
    idx = np.arange(len(all_h5))
    test = np.random.choice(idx, size=n_test, replace=False)
    rem = np.setdiff1d(idx, test)
    val = np.random.choice(rem, size=n_val, replace=False)
    train = np.setdiff1d(rem, val)
    return ([all_h5[i] for i in train],
            [all_h5[i] for i in test],
            [all_h5[i] for i in val])


def main():
    parser = argparse.ArgumentParser(description='Generate ML data grids with precise progress logs')
    parser.add_argument('input_dir')
    parser.add_argument('output_dir')
    parser.add_argument('-n', '--nFiles', type=int, default=None,
                        help='Max files to process')
    parser.add_argument('--gridSize', type=int, nargs=3, default=[80,80,80])
    parser.add_argument('--minNHits', type=int, default=0)
    parser.add_argument('--minPrimarySteps', type=int, default=30)
    parser.add_argument('--primaryPdg', type=int, default=13)
    parser.add_argument('--noCuts', action='store_true')
    parser.add_argument('--noCheckFiles', action='store_true')
    parser.add_argument('--useHistograms', action='store_true')
    parser.add_argument('--overwriteDF', action='store_true')
    parser.add_argument('--saveDFhits', action='store_true')
    parser.add_argument('--saveDFprimary', action='store_true')
    parser.add_argument('--saveGridNpyHits', action='store_true')
    parser.add_argument('--saveGridNpyPrimary', action='store_true')
    parser.add_argument('--saveGridH5', action='store_true')
    parser.add_argument('--numWorkers', type=int, default=1)
    parser.add_argument('--nTest', type=int, default=20)
    parser.add_argument('--nVal', type=int, default=10)
    parser.add_argument('--noSplit', action='store_true')
    parser.add_argument('--saveAsNumbersOnly-H5', action='store_true')
    parser.add_argument('-v','--verbosity', choices=['debug','info','warning','error','critical'],
                        default='info')
    args = parser.parse_args()

    configure_logging(args.verbosity)
    LOGGER.info("Starting data grid generation")

    os.makedirs(args.output_dir, exist_ok=True)

    files = sorted([f for f in os.listdir(args.input_dir) if f.endswith('.root')])
    if args.nFiles:
        files = files[:args.nFiles]
        LOGGER.info(f"Limiting to first {len(files)} files")
    paths = [os.path.join(args.input_dir, f) for f in files]

    if not args.noCheckFiles:
        paths = check_files(paths, 'photoSensor_hits_histograms', args.numWorkers)
    if not args.noCuts:
        paths = apply_cuts(paths, args.minNHits, args.minPrimarySteps,
                            'photoSensor_hits_histograms', 'primary;1', args.numWorkers)

    need_h = args.saveDFhits or args.saveGridNpyHits or args.saveGridH5
    need_p = args.saveDFprimary or args.saveGridNpyPrimary or args.saveGridH5

    dfh_paths = []
    dfp_paths = []

    with tempfile.TemporaryDirectory() as tmp:
        hb = (os.path.join(args.output_dir, 'DF_hits') if args.saveDFhits else os.path.join(tmp, 'DF_hits'))
        pb = (os.path.join(args.output_dir, 'DF_primary') if args.saveDFprimary else os.path.join(tmp, 'DF_primary'))

        if need_h:
            os.makedirs(os.path.dirname(hb), exist_ok=True)
            dfh_paths = process_hits_df(paths, 'photoSensor_hits_histograms',
                                        hb, args.overwriteDF, args.useHistograms,
                                        args.numWorkers)
        if need_p:
            os.makedirs(os.path.dirname(pb), exist_ok=True)
            dfp_paths = process_primary_df(paths, 'primary;1',
                                           pb, args.overwriteDF, args.primaryPdg,
                                           args.numWorkers)

        gs = tuple(args.gridSize)
        bh_n = os.path.join(args.output_dir, 'grid_hits')
        bp_n = os.path.join(args.output_dir, 'grid_primary')
        bh5 = os.path.join(args.output_dir, 'grid_h5')
        npy_h = [f"{bh_n}_{i}.npy" for i in range(len(dfh_paths))]
        npy_p = [f"{bp_n}_{i}.npy" for i in range(len(dfp_paths))]
        h5s = []
        if args.saveAsNumbersOnly_H5:
            h5s = [f"{i}.h5" for i in range(len(dfh_paths))]
        else:
            h5s = [f"{bh5}_{i}.h5" for i in range(len(dfh_paths))]

        if args.saveGridNpyHits or args.saveGridNpyPrimary or args.saveGridH5:
            create_grids(dfh_paths, dfp_paths,
                         npy_h, npy_p, h5s,
                         args.saveGridNpyHits, args.saveGridNpyPrimary,
                         args.saveGridH5,
                         gs, combine_errors=False,
                         combine_walls=True,
                         walls_combine=False,
                         walls_method=expNWalls,
                         combine_vectors=True,
                         num_workers=args.numWorkers)

        if not args.noSplit and args.saveGridH5:
            train, test, val = split_data(h5s, args.nTest, args.nVal, seed=42)
            for name, lst in [('train', train), ('test', test), ('val', val)]:
                of = os.path.join(args.output_dir, f"{name}_paths.txt")
                with open(of, 'w') as f:
                    f.write("\n".join(lst))
                LOGGER.info(f"Wrote {name} paths: {of}")

    LOGGER.info("Data grid generation completed")

if __name__ == '__main__':
    main()
