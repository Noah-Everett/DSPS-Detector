#!/usr/bin/env python3
"""
makeDataGrids.py

End-to-end pipeline to:
- Process ROOT files into hits/primary DataFrames
- (Optionally) create dense voxel grids (x/y) and write to .npy/.h5
- (Optionally) create sparse per-photon voxel paths DataFrames using gridMethods.get_voxelGrid
- Split dataset into train/val/test deterministically
- Emit a manifest mapping input files to outputs for auditability

Key points:
- Voxel paths are derived by calling get_voxelGrid(..., return_individual=True [, return_distances=True])
  and converting per-hit grids into a sparse coordinate list.
- No custom traversal: we reuse your fast voxel traversal through gridMethods.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import logging
import os
import sys
import gc
import time
import tempfile
from typing import List, Tuple, Optional, Dict

import numpy as np
import pandas as pd
import h5py
import uproot
from tqdm import tqdm
from joblib import Parallel, delayed

# allow imports from parent python directory which is ../python
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'python')))
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

# IMPORTANT: import exactly what we need from the file you provided
from gridMethods import (
    get_voxelGrid,
    make_voxelGrid_truth,
    expNWalls,
    wallStringToInt,
)

# -----------------------
# Parquet configuration
# -----------------------
ROW_GROUP_SIZE = 256_000  # good general default
try:
    import pyarrow as _pa  # noqa: F401
    import pyarrow.parquet as _pq  # noqa: F401
    PARQUET_ENGINE = "pyarrow"
    PARQUET_COMPRESSION = "zstd"
except Exception:
    try:
        import fastparquet  # noqa: F401
        PARQUET_ENGINE = "fastparquet"
        PARQUET_COMPRESSION = "zstd"
    except Exception:
        PARQUET_ENGINE = None
        PARQUET_COMPRESSION = None  # let pandas/engine decide


def _parquet_kwargs():
    kw = dict(index=False)
    if PARQUET_ENGINE:
        kw["engine"] = PARQUET_ENGINE
    if PARQUET_COMPRESSION:
        kw["compression"] = PARQUET_COMPRESSION
    return kw


def save_parquet(df: pd.DataFrame, path: str, metadata: Optional[Dict[str, str]] = None):
    """
    Robust parquet writer with optional metadata.
    - Uses pyarrow with row groups and metadata if available.
    - Otherwise falls back to pandas.to_parquet with best-effort compression.
    """
    if PARQUET_ENGINE == "pyarrow":
        import pyarrow as pa
        import pyarrow.parquet as pq
        table = pa.Table.from_pandas(df, preserve_index=False)
        if metadata:
            meta_bytes = {str(k): str(v).encode("utf-8") for k, v in metadata.items()}
            existing = table.schema.metadata or {}
            table = table.replace_schema_metadata({**existing, **meta_bytes})
        pq.write_table(table, path, compression=PARQUET_COMPRESSION, row_group_size=ROW_GROUP_SIZE)
    else:
        df.to_parquet(path, **_parquet_kwargs())


# -----------------------
# Logging
# -----------------------
LOGGER = logging.getLogger("makeDataGrids")


def configure_logging(verbosity: str):
    level = getattr(logging, verbosity.upper(), None)
    if not isinstance(level, int):
        raise ValueError(f"Invalid verbosity level: {verbosity}")
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(name)s - %(filename)s::%(funcName)s(%(lineno)d) - %(levelname)s - %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S'
    )
    # quiet noisy libs
    logging.getLogger("fsspec.local").setLevel(logging.WARNING)
    logging.getLogger("numba.core.byteflow").setLevel(logging.WARNING)
    logging.getLogger("numba.core.ssa").setLevel(logging.WARNING)
    logging.getLogger("numba.core.interpreter").setLevel(logging.WARNING)
    logging.getLogger("numba.core.entrypoints").setLevel(logging.WARNING)


# -----------------------
# Utility functions
# -----------------------
def r_to_theta(r: float) -> float:
    """Convert radius to theta using constants."""
    return r / (CM_PER_RAD * MM_PER_CM)


def validate_geometry(grid_shape: Tuple[int, int, int], dims_mm: Tuple[float, float, float]):
    gs = np.asarray(grid_shape, dtype=int)
    dm = np.asarray(dims_mm, dtype=float)
    assert gs.shape == (3,), "gridSize must be 3D"
    assert dm.shape == (3,), "DETECTOR_SIZE_MM must be 3D"
    if not (gs > 0).all():
        raise ValueError(f"gridSize must be positive in all dims, got {gs}")
    if not (dm > 0).all():
        raise ValueError(f"DETECTOR_SIZE_MM must be positive in all dims, got {dm}")
    vox = dm / gs
    if (vox < 0.5).any():
        LOGGER.warning(f"Very fine voxels {vox} mm — memory/time may spike.")
    return vox


def _stable_hash(s: str, seed: int = 42) -> int:
    return int(hashlib.sha1((str(seed) + s).encode()).hexdigest(), 16)


def split_data_stable(paths: List[str], p_test: float = 0.2, p_val: float = 0.1, seed: int = 42):
    """
    Deterministically assign each path to train/test/val by hashing the basename.
    Returns (train, test, val) lists.
    """
    test, val, train = [], [], []
    denom = float(2**160)
    for p in paths:
        h = _stable_hash(os.path.basename(p), seed) / denom  # uniform in [0,1)
        if h < p_test:
            test.append(p)
        elif h < p_test + p_val:
            val.append(p)
        else:
            train.append(p)
    return train, test, val


def _should_write(path: str, force: bool, resume: bool) -> bool:
    exists = os.path.exists(path)
    if force:
        return True
    if resume:
        return not exists
    return not exists


def safe_get(lst: List[str], i: int) -> Optional[str]:
    return lst[i] if 0 <= i < len(lst) else None


# -----------------------
# ROOT file checks & cuts
# -----------------------
def check_files(paths: List[str], hist_dir: str, num_workers: int) -> List[str]:
    """Verify that each ROOT file contains histograms of the same shape."""
    if not paths:
        return []
    def _shape(path):
        with uproot.open(path, num_workers=num_workers) as f:
            h = f[hist_dir]
            first_key = next(iter(h.keys()))
            return h[first_key].values().shape

    try:
        expected = _shape(paths[0])
        LOGGER.info(f"Expected histogram shape from {paths[0]}: {expected}")
    except Exception as e:
        LOGGER.error(f"Cannot read {hist_dir} in {paths[0]}: {e}")
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


def apply_cuts(paths: List[str], min_hits: int, min_primary_steps: int,
               hist_dir: str, primary_tree: str, num_workers: int) -> List[str]:
    """Filter files by total hit count and number of primary steps."""
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


# -----------------------
# DataFrame builders
# -----------------------
def process_hits_df(paths: List[str], hist_dir: str, output_base: str,
                    use_histograms: bool, num_workers: int,
                    force: bool, resume: bool) -> List[str]:
    """Parallel build & save hits DataFrames; returns list of parquet paths."""
    LOGGER.info(f"Processing {len(paths)} hits DataFrames...")

    def _build(i, path):
        out_path = f"{output_base}_{i}.parquet"
        if not _should_write(out_path, force, resume):
            LOGGER.debug(f"Skipping existing hits DF: {out_path}")
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

        # feature engineering
        df = make_r(df)
        df = filter_r(df, Y_LIM)
        df = make_theta(df, r_to_theta)
        df = make_phi(df)
        df = make_reconstructedVector_direction(df)
        if 'initialPosition' in df.columns:
            df = make_relativeVector(df)

        save_parquet(df, out_path)
        LOGGER.debug(f"Saved hits DF: {out_path}")

        del df
        gc.collect()
        return out_path

    paths_out = Parallel(n_jobs=num_workers)(
        delayed(_build)(i, p) for i, p in enumerate(tqdm(paths, desc="Building hits DFs"))
    )
    return paths_out


def process_primary_df(paths: List[str], primary_tree: str, output_base: str,
                       overwrite_persisted: bool, pdg_code: int, num_workers: int,
                       force: bool, resume: bool) -> List[str]:
    """Parallel build & save primary DataFrames; returns list of parquet paths."""
    LOGGER.info(f"Processing {len(paths)} primary DataFrames...")

    def _build(i, path):
        out_path = f"{output_base}_{i}.parquet"
        if not _should_write(out_path, force, resume):
            LOGGER.debug(f"Skipping existing primary DF: {out_path}")
            return out_path

        positions = get_primary_position(path, primary_tree)
        pdgs = get_primary_pdg(path, primary_tree)
        df = pd.DataFrame({'position': positions, 'pdg': pdgs})

        half = np.array(DETECTOR_SIZE_MM) / 2.0
        df = df[df['pdg'] == pdg_code]
        df = df[df['position'].apply(lambda xyz: all(-half[j] < xyz[j] < half[j] for j in range(3)))]

        save_parquet(df, out_path)
        LOGGER.debug(f"Saved primary DF: {out_path}")

        del df, positions, pdgs
        gc.collect()
        return out_path

    paths_out = Parallel(n_jobs=num_workers)(
        delayed(_build)(i, p) for i, p in enumerate(tqdm(paths, desc="Building primary DFs"))
    )
    return paths_out


# -----------------------
# Dense voxel grids (reuse gridMethods.get_voxelGrid)
# -----------------------
def create_grids(df_hits: List[str], df_pri: List[str],
                 npy_hits: List[str], npy_pri: List[str], h5_paths: List[str],
                 save_npy_hits: bool, save_npy_pri: bool, save_h5: bool,
                 grid_shape: Tuple[int, int, int], combine_errors: bool, combine_walls: bool,
                 walls_combine: bool, walls_method, combine_vectors: bool,
                 num_workers: int, force: bool, resume: bool):
    """Parallel create voxel grids and save as .npy/.h5."""
    LOGGER.info("Creating voxel grids...")
    jobs = list(zip(df_hits, df_pri, npy_hits, npy_pri, h5_paths))

    def _make(idx, dfh_p, dfp_p, nh, npf, h5p):
        do_npy_h = save_npy_hits and _should_write(nh, force, resume)
        do_npy_p = save_npy_pri and _should_write(npf, force, resume)
        do_h5    = save_h5 and _should_write(h5p, force, resume)

        if not (do_npy_h or do_npy_p or do_h5):
            return

        x = y = None
        if (save_npy_hits or save_h5) and dfh_p is not None:
            dfh = pd.read_parquet(dfh_p)
            starts = np.vstack(dfh['sensor_position'].tolist())
            dirs = -np.vstack(dfh['reconstructedVector_direction'].tolist())
            x = get_voxelGrid(
                grid_shape=np.array(grid_shape),
                grid_dimensions=np.array(DETECTOR_SIZE_MM, dtype=float),
                vector_starts=starts,
                vector_directions=dirs,
                vector_weights=None,
                vector_start_walls=wallStringToInt(dfh['sensor_wall'].to_numpy().astype(str)),
                walls=combine_walls,
                walls_combine=walls_combine,
                walls_combine_method=walls_method,
                vector_combine=combine_vectors,
                use_distance=False,
                return_individual=False,
                return_distances=False,
            )
            del dfh, starts, dirs
            gc.collect()

        if (save_npy_pri or save_h5) and dfp_p is not None:
            dfp = pd.read_parquet(dfp_p)
            pos = np.vstack(dfp['position'].tolist()) if len(dfp) else np.empty((0, 3))
            y = make_voxelGrid_truth(pos, shape=grid_shape,
                                     detectorDimensions=DETECTOR_SIZE_MM,
                                     makeErrors=False)[0]
            del dfp, pos
            gc.collect()

        if do_h5:
            with h5py.File(h5p, 'w') as f:
                if x is not None:
                    f.create_dataset('x', data=np.moveaxis(x, -1, 0))  # channel-first
                if y is not None:
                    f.create_dataset('y', data=y)
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


# -----------------------
# Sparse voxel paths via gridMethods.get_voxelGrid (batched)
# -----------------------
def _paths_from_individual_grids(gi: np.ndarray,
                                 gdi: Optional[np.ndarray],
                                 start_hit_idx: int,
                                 include_distance: bool) -> List[tuple]:
    """
    Convert per-hit grids (and optional per-hit distance grids) into sparse rows.
    Returns a list of tuples:
      (photon_idx, ix, iy, iz[, dist_mm])
    """
    rows = []
    n_hits = gi.shape[0]
    # Iterate per-hit; use .nonzero to get traversed voxels.
    for local_i in range(n_hits):
        nz = np.nonzero(gi[local_i])
        ix, iy, iz = nz[0], nz[1], nz[2]
        if ix.size == 0:
            continue
        photon_idx = start_hit_idx + local_i
        if include_distance and gdi is not None:
            ds = gdi[local_i, ix, iy, iz]
            rows.extend((photon_idx, int(ix[k]), int(iy[k]), int(iz[k]), float(ds[k])) for k in range(ix.size))
        else:
            rows.extend((photon_idx, int(ix[k]), int(iy[k]), int(iz[k])) for k in range(ix.size))
    return rows


def save_voxel_paths(df_hits_paths: List[str], output_parquet_paths: List[str],
                     grid_shape: Tuple[int, int, int], include_distance: bool,
                     num_workers: int, force: bool, resume: bool,
                     batch_size: int = 2000):
    """
    For each hits DF:
      - Build per-hit grids in batches using gridMethods.get_voxelGrid(return_individual=True)
      - Convert to sparse (photon_idx, ix, iy, iz[, dist_mm]) rows
      - Save as Parquet
    We avoid custom traversal and reuse your grid backend for perfect parity.
    """
    LOGGER.info("Computing per-photon voxel paths using gridMethods.get_voxelGrid (batched)...")
    jobs = list(zip(df_hits_paths, output_parquet_paths))

    vx_size = (np.asarray(DETECTOR_SIZE_MM, float) / np.asarray(grid_shape, int))
    meta = {
        "voxel_size_mm_x": vx_size[0],
        "voxel_size_mm_y": vx_size[1],
        "voxel_size_mm_z": vx_size[2],
        "grid_size_x": int(grid_shape[0]),
        "grid_size_y": int(grid_shape[1]),
        "grid_size_z": int(grid_shape[2]),
        "detector_size_mm_x": float(DETECTOR_SIZE_MM[0]),
        "detector_size_mm_y": float(DETECTOR_SIZE_MM[1]),
        "detector_size_mm_z": float(DETECTOR_SIZE_MM[2]),
        "notes": "Derived from per-hit grids returned by gridMethods.get_voxelGrid",
    }

    def _one(i, dfh_path, out_path):
        if not _should_write(out_path, force, resume):
            LOGGER.debug(f"Skipping existing voxel paths: {out_path}")
            return out_path

        dfh = pd.read_parquet(dfh_path)
        n_hits_total = len(dfh)
        if n_hits_total == 0:
            # Write empty schema
            cols = ['photon_idx', 'ix', 'iy', 'iz'] + (['dist_mm'] if include_distance else [])
            empty = pd.DataFrame(columns=cols)
            save_parquet(empty, out_path, metadata=meta)
            LOGGER.debug(f"Saved empty voxel paths DF: {out_path}")
            return out_path

        starts_all = np.vstack(dfh['sensor_position'].tolist())
        dirs_all = -np.vstack(dfh['reconstructedVector_direction'].tolist())

        # Optional event_id passthrough later
        event_col = None
        for candidate in ['event_id', 'event', 'track_id', 'track']:
            if candidate in dfh.columns:
                event_col = candidate
                break

        rows: List[tuple] = []
        # Process in batches to keep memory bounded
        for start in tqdm(range(0, n_hits_total, batch_size), desc=f"Voxel paths ({os.path.basename(dfh_path)})", leave=False):
            end = min(start + batch_size, n_hits_total)
            starts = starts_all[start:end]
            dirs = dirs_all[start:end]

            # Request per-hit grids (weighted=1 per voxel when use_distance=False)
            # If include_distance, also return per-hit distance grids.
            if include_distance:
                # get_voxelGrid returns: grid, grids_individual, grid_distance, grids_distance_individual
                _, gi, _, gdi = get_voxelGrid(
                    grid_shape=np.array(grid_shape),
                    grid_dimensions=np.array(DETECTOR_SIZE_MM, dtype=float),
                    vector_starts=starts,
                    vector_directions=dirs,
                    vector_weights=None,
                    vector_start_walls=None,
                    walls=False,
                    walls_combine=False,
                    walls_combine_method=None,
                    vector_combine=False,
                    use_distance=False,              # keep weighted grids as 1-per-voxel; distances come from gdi
                    return_individual=True,
                    return_distances=True,
                )
            else:
                # get_voxelGrid returns: grid, grids_individual
                _, gi = get_voxelGrid(
                    grid_shape=np.array(grid_shape),
                    grid_dimensions=np.array(DETECTOR_SIZE_MM, dtype=float),
                    vector_starts=starts,
                    vector_directions=dirs,
                    vector_weights=None,
                    vector_start_walls=None,
                    walls=False,
                    walls_combine=False,
                    walls_combine_method=None,
                    vector_combine=False,
                    use_distance=False,
                    return_individual=True,
                    return_distances=False,
                )
                gdi = None

            # Convert batch per-hit grids -> sparse rows
            rows.extend(_paths_from_individual_grids(gi, gdi, start_hit_idx=start, include_distance=include_distance))

            # free batch memory
            del gi, gdi
            gc.collect()

        # Build DataFrame
        if include_distance:
            df_paths = pd.DataFrame(rows, columns=['photon_idx', 'ix', 'iy', 'iz', 'dist_mm'])
        else:
            df_paths = pd.DataFrame(rows, columns=['photon_idx', 'ix', 'iy', 'iz'])

        # Add stable file_id and optional event_id passthrough
        file_id = os.path.splitext(os.path.basename(dfh_path))[0]
        df_paths['file_id'] = file_id
        if event_col is not None and len(df_paths):
            mapping = pd.Series(dfh[event_col].values).to_dict()  # local row index -> event_id-like
            df_paths['event_id'] = df_paths['photon_idx'].map(mapping)

        # Reorder columns for readability
        lead_cols = ['file_id']
        if 'event_id' in df_paths.columns:
            lead_cols.append('event_id')
        lead_cols.append('photon_idx')
        remainder = [c for c in df_paths.columns if c not in lead_cols]
        df_paths = df_paths[lead_cols + remainder]

        save_parquet(df_paths, out_path, metadata=meta)
        LOGGER.debug(f"Saved voxel paths DF: {out_path}")

        del dfh, starts_all, dirs_all, df_paths, rows
        gc.collect()
        return out_path

    Parallel(n_jobs=num_workers)(
        delayed(_one)(i, *job) for i, job in enumerate(tqdm(jobs, desc="Saving voxel paths"))
    )


# -----------------------
# Dense-from-sparse reconstructor (optional helper)
# -----------------------
def voxel_paths_to_dense(df_paths: pd.DataFrame, grid_shape: Tuple[int, int, int], use_distance: bool = False) -> np.ndarray:
    """Rebuild a dense grid from a voxel-paths DF for validation or analysis."""
    grid = np.zeros(grid_shape, dtype=float)
    if len(df_paths) == 0:
        return grid
    ix = df_paths['ix'].to_numpy(dtype=int)
    iy = df_paths['iy'].to_numpy(dtype=int)
    iz = df_paths['iz'].to_numpy(dtype=int)
    if use_distance and 'dist_mm' in df_paths.columns:
        w = df_paths['dist_mm'].to_numpy(dtype=float)
    else:
        w = np.ones(len(df_paths), dtype=float)
    np.add.at(grid, (ix, iy, iz), w)
    return grid


# -----------------------
# Main
# -----------------------
def main():
    parser = argparse.ArgumentParser(description='Generate ML data grids and/or sparse voxel paths with precise progress logs')
    parser.add_argument('input_dir')
    parser.add_argument('output_dir')
    parser.add_argument('-n', '--nFiles', type=int, default=None, help='Max files to process')

    # Geometry & data
    parser.add_argument('--gridSize', type=int, nargs=3, default=[80, 80, 80])
    parser.add_argument('--minNHits', type=int, default=0)
    parser.add_argument('--minPrimarySteps', type=int, default=30)
    parser.add_argument('--primaryPdg', type=int, default=13)

    # Pipeline toggles
    parser.add_argument('--noCuts', action='store_true')
    parser.add_argument('--noCheckFiles', action='store_true')
    parser.add_argument('--useHistograms', action='store_true')

    # Persistence & behavior
    parser.add_argument('--saveDFhits', action='store_true')
    parser.add_argument('--saveDFprimary', action='store_true')
    parser.add_argument('--saveGridNpyHits', action='store_true')
    parser.add_argument('--saveGridNpyPrimary', action='store_true')
    parser.add_argument('--saveGridH5', action='store_true')

    # Sparse voxel paths (via gridMethods.get_voxelGrid)
    parser.add_argument('--saveVoxelPaths', action='store_true',
                        help='Save a per-photon DataFrame of voxel indices (and distances) traversed')
    parser.add_argument('--voxelPathsUseDistance', action='store_true',
                        help='If set, include distance traveled inside each voxel (mm)')
    parser.add_argument('--voxelPathsBatch', type=int, default=2000,
                        help='Batch size for building per-hit grids to derive voxel paths')

    # Split settings (stable hashing)
    parser.add_argument('--noSplit', action='store_true')
    parser.add_argument('--pTest', type=float, default=0.20, help='Fraction for test split')
    parser.add_argument('--pVal', type=float, default=0.10, help='Fraction for validation split')
    parser.add_argument('--splitSeed', type=int, default=42, help='Seed for stable hashing split')
    # Legacy counts (translated to fractions if provided)
    parser.add_argument('--nTest', type=int, default=None, help='(Legacy) Target size for test; converted to fraction')
    parser.add_argument('--nVal', type=int, default=None, help='(Legacy) Target size for val; converted to fraction')

    # Execution
    parser.add_argument('--numWorkers', type=int, default=1)
    parser.add_argument('--saveAsNumbersOnly-H5', action='store_true')

    # Orchestration
    parser.add_argument('--force', action='store_true', help='Recompute and overwrite all derived artifacts')
    parser.add_argument('--resume', action='store_true', help='Only compute missing outputs; never overwrite')

    parser.add_argument('-v', '--verbosity', choices=['debug', 'info', 'warning', 'error', 'critical'],
                        default='info')
    args = parser.parse_args()

    configure_logging(args.verbosity)
    t_start = time.time()
    LOGGER.info("Starting data generation")

    os.makedirs(args.output_dir, exist_ok=True)
    validate_geometry(tuple(args.gridSize), tuple(DETECTOR_SIZE_MM))

    # Gather inputs
    files = sorted([f for f in os.listdir(args.input_dir) if f.endswith('.root')])
    if args.nFiles:
        files = files[:args.nFiles]
        LOGGER.info(f"Limiting to first {len(files)} files")
    paths = [os.path.join(args.input_dir, f) for f in files]

    # Checks & cuts
    if not args.noCheckFiles:
        paths = check_files(paths, 'photoSensor_hits_histograms', args.numWorkers)
    if not args.noCuts:
        paths = apply_cuts(paths, args.minNHits, args.minPrimarySteps,
                           'photoSensor_hits_histograms', 'primary;1', args.numWorkers)
    if not paths:
        LOGGER.error("No valid input files remain after checks/cuts. Exiting.")
        return

    # Decide which DFs are needed (even if not persisted)
    need_h = args.saveDFhits or args.saveGridNpyHits or args.saveGridH5 or args.saveVoxelPaths
    need_p = args.saveDFprimary or args.saveGridNpyPrimary or args.saveGridH5

    dfh_paths: List[str] = []
    dfp_paths: List[str] = []

    manifest: List[Dict[str, Optional[str]]] = []

    with tempfile.TemporaryDirectory() as tmp:
        # Base names for DF outputs: persisted or ephemeral
        hb_base = (os.path.join(args.output_dir, 'DF_hits') if args.saveDFhits else os.path.join(tmp, 'DF_hits'))
        pb_base = (os.path.join(args.output_dir, 'DF_primary') if args.saveDFprimary else os.path.join(tmp, 'DF_primary'))

        if need_h:
            os.makedirs(os.path.dirname(hb_base), exist_ok=True)
            dfh_paths = process_hits_df(paths, 'photoSensor_hits_histograms',
                                        hb_base, args.useHistograms,
                                        args.numWorkers, force=args.force, resume=args.resume)
        if need_p:
            os.makedirs(os.path.dirname(pb_base), exist_ok=True)
            dfp_paths = process_primary_df(paths, 'primary;1',
                                           pb_base, overwrite_persisted=True, pdg_code=args.primaryPdg,
                                           num_workers=args.numWorkers, force=args.force, resume=args.resume)

        # Prepare outputs for grids
        gs = tuple(args.gridSize)
        bh_n = os.path.join(args.output_dir, 'grid_hits')
        bp_n = os.path.join(args.output_dir, 'grid_primary')
        npy_h = [f"{bh_n}_{i}.npy" for i in range(len(dfh_paths))] if (args.saveGridNpyHits or args.saveGridH5) else []
        npy_p = [f"{bp_n}_{i}.npy" for i in range(len(dfp_paths))] if (args.saveGridNpyPrimary or args.saveGridH5) else []

        if args.saveAsNumbersOnly_H5:
            h5s = [os.path.join(args.output_dir, f"{i}.h5") for i in range(len(dfh_paths))]
        else:
            bh5 = os.path.join(args.output_dir, 'grid_h5')
            h5s = [f"{bh5}_{i}.h5" for i in range(len(dfh_paths))] if (args.saveGridH5) else []

        # Compute dense grids if requested
        if args.saveGridNpyHits or args.saveGridNpyPrimary or args.saveGridH5:
            if not (dfh_paths and dfp_paths):
                LOGGER.warning("Grid creation requested but hits/primary DFs are missing. Skipping grids.")
            else:
                create_grids(dfh_paths, dfp_paths,
                             npy_h, npy_p, h5s,
                             args.saveGridNpyHits, args.saveGridNpyPrimary,
                             args.saveGridH5,
                             gs, combine_errors=False,
                             combine_walls=True,
                             walls_combine=False,
                             walls_method=expNWalls,
                             combine_vectors=True,
                             num_workers=args.numWorkers, force=args.force, resume=args.resume)

        # Compute sparse voxel paths if requested
        voxel_paths_out: List[str] = []
        if args.saveVoxelPaths:
            vp_base = os.path.join(args.output_dir, 'voxel_paths')
            os.makedirs(os.path.dirname(vp_base), exist_ok=True)
            voxel_paths_out = [f"{vp_base}_{i}.parquet" for i in range(len(dfh_paths))]
            save_voxel_paths(df_hits_paths=dfh_paths,
                             output_parquet_paths=voxel_paths_out,
                             grid_shape=gs,
                             include_distance=args.voxelPathsUseDistance,
                             num_workers=args.numWorkers,
                             force=args.force, resume=args.resume,
                             batch_size=args.voxelPathsBatch)

        # Train/test/val split (stable) — only when we produced H5 grids
        if not args.noSplit and args.saveGridH5:
            p_test = float(args.pTest)
            p_val = float(args.pVal)
            if args.nTest is not None and len(h5s) > 0:
                p_test = max(0.0, min(1.0, args.nTest / float(len(h5s))))
                LOGGER.info(f"Using legacy nTest={args.nTest} -> pTest≈{p_test:.3f}")
            if args.nVal is not None and len(h5s) > 0:
                p_val = max(0.0, min(1.0, args.nVal / float(len(h5s))))
                LOGGER.info(f"Using legacy nVal={args.nVal} -> pVal≈{p_val:.3f}")
            if p_test + p_val >= 1.0:
                raise ValueError(f"pTest + pVal must be < 1.0 (got {p_test+p_val})")
            train, test, val = split_data_stable(h5s, p_test=p_test, p_val=p_val, seed=args.splitSeed)
            for name, lst in [('train', train), ('test', test), ('val', val)]:
                of = os.path.join(args.output_dir, f"{name}_paths.txt")
                with open(of, 'w') as f:
                    f.write("\n".join(lst))
                LOGGER.info(f"Wrote {name} paths: {of}")

        # Build manifest: one row per input (post-cuts)
        N = len(paths)
        for i in range(N):
            manifest.append({
                "input_root": paths[i],
                "df_hits": (safe_get(dfh_paths, i) if args.saveDFhits else None),
                "df_primary": (safe_get(dfp_paths, i) if args.saveDFprimary else None),
                "grid_hits_npy": (safe_get(npy_h, i) if args.saveGridNpyHits else None),
                "grid_primary_npy": (safe_get(npy_p, i) if args.saveGridNpyPrimary else None),
                "grid_h5": (safe_get(h5s, i) if args.saveGridH5 else None),
                "voxel_paths": (safe_get(voxel_paths_out, i) if args.saveVoxelPaths else None),
            })

    # Write manifest (outside tempdir scope)
    manifest_path = os.path.join(args.output_dir, "manifest.json")
    with open(manifest_path, "w") as f:
        json.dump(manifest, f, indent=2)
    LOGGER.info(f"Wrote manifest: {manifest_path}")

    dt = time.time() - t_start
    n_dfh = sum(1 for m in manifest if m["df_hits"])
    n_dfp = sum(1 for m in manifest if m["df_primary"])
    n_h5 = sum(1 for m in manifest if m["grid_h5"])
    n_vp = sum(1 for m in manifest if m["voxel_paths"])
    LOGGER.info(f"Completed in {dt:.1f}s — hitsDF={n_dfh}, primaryDF={n_dfp}, H5={n_h5}, voxelPaths={n_vp}")


if __name__ == '__main__':
    main()