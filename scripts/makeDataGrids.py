#!/usr/bin/env python3
# =============================================================================
# makeDataGrids.py
# =============================================================================
# End-to-end pipeline to:
#   • Read ROOT files and build per-file hits / primary DataFrames
#   • (Optional) Create dense voxel grids (x = hits-reconstruction, y = truth)
#   • (Optional) Create *sparse* per-photon voxel paths using a fast voxel
#     traversal (FVT) **without** ever materializing per-hit dense tensors
#   • Deterministically split H5 grid files into train/val/test sets
#   • Emit a manifest JSON that maps input files → output artifacts for auditing
#
# Key design choices (performance / clarity):
#   • All voxel enumeration is centralized in `gridMethods.fvt_intersections_basic`.
#     This runs the fast-voxel-traversal once and returns:
#       (ray_index, ix, iy, iz, t_enter_u, t_exit_u, dist_mm)
#     for every voxel a finite segment crosses.
#   • Sparse voxel paths are built by streaming those rows directly to a compact
#     Parquet table, avoiding per-hit 4D dense arrays (N × gx × gy × gz).
#   • Dense voxel grids (for legacy training) still use `gridMethods.get_voxelGrid`
#     to preserve exact historical behavior (walls, optional distance weighting,
#     vector combining, etc.).
#   • Defensive IO: pyarrow (+zstd) preferred; falls back gracefully when missing.
#   • Thorough logging and verbose docstrings to make each step explicit.
# =============================================================================

from __future__ import annotations

import argparse
import hashlib
import json
import logging
import os
import sys
import gc
import time
from typing import List, Tuple, Optional, Dict

import numpy as np
import pandas as pd
import h5py
import uproot
from tqdm import tqdm
from joblib import Parallel, delayed

# -----------------------------------------------------------------------------
# Import project-local dependencies
# -----------------------------------------------------------------------------
# Allow imports from parent python directory
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'python')))

# Geometry and constants
from constants import CM_PER_RAD, MM_PER_CM, Y_LIM, DETECTOR_SIZE_MM

# ROOT reading helpers
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

# Feature engineering helpers
from hitAccuracyMethods import (
    make_r, make_theta, make_phi,
    make_reconstructedVector_direction, make_relativeVector
)
from filterMethods import filter_r

# Voxel/grid machinery (now layered on top of a single FVT base)
from gridMethods import (
    # Centralized fast voxel traversal primitive (the “most basic FVT”)
    fvt_intersections_basic,
    # High-level grid builders and geometry helpers
    get_voxelGrid,
    make_voxelGrid_truth,
    expNWalls,
    wallStringToInt,
    get_vectors_end,
)

# =============================================================================
# Parquet configuration (robust writer that prefers pyarrow+zstd)
# =============================================================================
ROW_GROUP_SIZE = 256_000  # Reasonable default for large sparse tables
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
        PARQUET_COMPRESSION = None  # Let pandas decide


def _parquet_kwargs():
    """Small helper to inject consistent defaults to pandas.to_parquet."""
    kw = dict(index=False)
    if PARQUET_ENGINE:
        kw["engine"] = PARQUET_ENGINE
    if PARQUET_COMPRESSION:
        kw["compression"] = PARQUET_COMPRESSION
    return kw


def save_parquet(df: pd.DataFrame, path: str, metadata: Optional[Dict[str, str]] = None):
    """
    Write a pandas DataFrame to Parquet robustly:

    • If pyarrow is available, write with row groups and attach file-level metadata.
    • Else fall back to pandas/fastparquet with best-effort compression.
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


# =============================================================================
# Logging
# =============================================================================
LOGGER = logging.getLogger("makeDataGrids")


def configure_logging(verbosity: str):
    """
    Configure global logging with a consistent, verbose format.

    Parameters
    ----------
    verbosity : {'debug','info','warning','error','critical'}
        Logging level selector exposed on the CLI.
    """
    level = getattr(logging, verbosity.upper(), None)
    if not isinstance(level, int):
        raise ValueError(f"Invalid verbosity level: {verbosity}")
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(name)s - %(filename)s::%(funcName)s(%(lineno)d) - %(levelname)s - %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S'
    )
    # Quiet noisy libs that tend to spam at DEBUG
    logging.getLogger("fsspec.local").setLevel(logging.WARNING)
    logging.getLogger("numba.core.byteflow").setLevel(logging.WARNING)
    logging.getLogger("numba.core.ssa").setLevel(logging.WARNING)
    logging.getLogger("numba.core.interpreter").setLevel(logging.WARNING)
    logging.getLogger("numba.core.entrypoints").setLevel(logging.WARNING)


# =============================================================================
# Utility / hygiene
# =============================================================================
def r_to_theta(r: float) -> float:
    """
    Convert cylindrical radius (mm) to polar angle theta (rad) using project constants.

    theta ≈ r / (cm_per_rad × mm_per_cm)
    """
    return r / (CM_PER_RAD * MM_PER_CM)


def validate_geometry(grid_shape: Tuple[int, int, int], dims_mm: Tuple[float, float, float]) -> np.ndarray:
    """
    Sanity-check grid shape and physical detector size, and return the voxel size.

    Emits a warning for extremely fine voxels which can inflate memory/time.
    """
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
    """
    SHA-1–based hashing for stable train/test/val assignment.

    Using SHA-1 ensures platform- and interpreter-stable results.
    """
    return int(hashlib.sha1((str(seed) + s).encode()).hexdigest(), 16)


def split_data_stable(paths: List[str], p_test: float, p_val: float, seed: int = 42):
    """
    Deterministically partition a list of paths into (train, test, val) by hashing.

    Parameters
    ----------
    paths : list of str
        Items to split (H5 file paths).
    p_test, p_val : float in [0,1)
        Fractions assigned to test and val, respectively.
    seed : int
        Seed mixed into the hash for reproducibility.

    Returns
    -------
    train, test, val : lists of str
        Disjoint splits that cover the inputs.
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
    """
    Decide whether we should write an output artifact.

    • force=True     → always write (recompute/overwrite)
    • resume=True    → write only if missing (never overwrite)
    • neither set    → write only if missing (conservative default)
    """
    exists = os.path.exists(path)
    if force:
        return True
    if resume:
        return not exists
    return not exists


def safe_get(lst: List[str], i: int) -> Optional[str]:
    """Return lst[i] if available, else None (convenience for manifest assembly)."""
    return lst[i] if 0 <= i < len(lst) else None


# =============================================================================
# ROOT file validation and selection (parallelized)
# =============================================================================
def check_files(paths: List[str], hist_dir: str, num_workers: int) -> List[str]:
    """
    Verify that each ROOT file contains histograms of the same shape.

    Rationale: downstream code assumes consistent binning across files. This
    function peeks at the first histogram under `hist_dir` to determine the
    expected shape, and then checks all files in parallel.
    """
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
    """
    Filter files by total hit count and number of primary steps.

    This is a coarse pruning step to skip extremely small/noisy files early.
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


# =============================================================================
# DataFrame builders (per-file, parallelized)
# =============================================================================
def process_hits_df(paths: List[str], hist_dir: str, output_base: str,
                    use_histograms: bool, num_workers: int,
                    force: bool, resume: bool) -> List[str]:
    """
    Build and save per-file **hits** DataFrames.

    The hits DF schema (subset) includes:
      • sensor_name (optional)
      • sensor_direction
      • sensor_position
      • sensor_wall
      • relativePosition_binned / relativePosition_nBin
      • engineered columns: r, theta, phi, reconstructedVector_direction
      • optional relativeVector (if initialPosition available)

    Returns
    -------
    List[str] : list of parquet paths (one per input file), in the same order.
    """
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

        # Feature engineering and cuts-by-radius
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
                       pdg_code: int, num_workers: int,
                       force: bool, resume: bool) -> List[str]:
    """
    Build and save per-file **primary** DataFrames filtered to the PDG of interest
    and constrained to lie within the detector bounds.

    The primary DF schema (subset) includes:
      • position : true primary positions (mm)
      • pdg      : PDG code
    """
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


# =============================================================================
# Dense voxel grids (legacy-compatible; uses gridMethods.get_voxelGrid)
# =============================================================================
def create_grids(df_hits: List[str], df_pri: List[str],
                 npy_hits: List[str], npy_pri: List[str], h5_paths: List[str],
                 save_npy_hits: bool, save_npy_pri: bool, save_h5: bool,
                 grid_shape: Tuple[int, int, int], combine_walls: bool,
                 walls_combine: bool, walls_method, combine_vectors: bool,
                 num_workers: int, force: bool, resume: bool):
    """
    Create dense voxel grids for x (reconstruction) and y (truth) in parallel.

    This path preserves historical semantics, including optional wall handling
    and vector combining. It is intentionally separate from the faster sparse
    voxel-path writer below.
    """
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

            # High-level grid builder (delegates traversal to FVT base)
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
            # Save in channel-first convention for x (if present)
            with h5py.File(h5p, 'w') as f:
                if x is not None:
                    f.create_dataset('x', data=np.moveaxis(x, -1, 0))
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


# =============================================================================
# Sparse voxel paths (fast): direct FVT enumeration → Parquet
# =============================================================================
def save_voxel_paths(df_hits_paths: List[str], output_parquet_paths: List[str],
                     grid_shape: Tuple[int, int, int], include_distance: bool,
                     num_workers: int, force: bool, resume: bool,
                     chunk_size: int = 20000):
    """
    Build *sparse* per-photon voxel paths using only the centralized FVT function.

    For each hits DF:
      1) Construct finite segments from (sensor_position, -reconstructed_dir)
         clipped to the detector box.
      2) Call `fvt_intersections_basic` in manageable chunks (to bound memory)
         to enumerate voxel crossings and segment distances.
      3) Convert to a Parquet table of rows:
           file_id, [event_id?], photon_idx, ix, iy, iz, [dist_mm?]
         where photon_idx is the row index in the hits DF.
      4) Write exactly once per file.

    This avoids allocating per-hit dense 4D tensors, making it dramatically
    faster and more memory-friendly for large N and high-resolution grids.
    """
    LOGGER.info("Computing per-photon voxel paths (streaming FVT, no dense per-hit tensors)...")
    jobs = list(zip(df_hits_paths, output_parquet_paths))

    grid_shape = np.asarray(grid_shape, int)
    det = np.asarray(DETECTOR_SIZE_MM, float)
    grid_min = -0.5 * det
    grid_max =  0.5 * det
    voxel_size = det / grid_shape

    meta = {
        "voxel_size_mm_x": float(voxel_size[0]),
        "voxel_size_mm_y": float(voxel_size[1]),
        "voxel_size_mm_z": float(voxel_size[2]),
        "grid_size_x": int(grid_shape[0]),
        "grid_size_y": int(grid_shape[1]),
        "grid_size_z": int(grid_shape[2]),
        "detector_size_mm_x": float(det[0]),
        "detector_size_mm_y": float(det[1]),
        "detector_size_mm_z": float(det[2]),
        "notes": "Sparse voxel paths via centralized FVT traversal; distances are geometric segment lengths in mm.",
    }

    def _one(i, dfh_path, out_path):
        if not _should_write(out_path, force, resume):
            LOGGER.debug(f"Skipping existing voxel paths: {out_path}")
            return out_path

        dfh = pd.read_parquet(dfh_path)
        n_hits_total = len(dfh)

        # Prepare output schema in case we need to write an empty table
        cols = ['file_id', 'photon_idx', 'ix', 'iy', 'iz']
        if include_distance:
            cols.append('dist_mm')

        # Try to pass through an event-id–like column if present
        event_col = next((c for c in ['event_id', 'event', 'track_id', 'track'] if c in dfh.columns), None)
        if event_col:
            cols.insert(1, 'event_id')  # after file_id for readability

        file_id = os.path.splitext(os.path.basename(dfh_path))[0]

        if n_hits_total == 0:
            empty = pd.DataFrame(columns=cols)
            save_parquet(empty, out_path, metadata=meta)
            LOGGER.debug(f"Saved empty voxel paths DF: {out_path}")
            return out_path

        # Build finite segments once as arrays (sensor → clipped end along -dir)
        starts_all = np.vstack(dfh['sensor_position'].tolist())
        dirs_all   = -np.vstack(dfh['reconstructedVector_direction'].tolist())
        ends_all   = get_vectors_end(starts_all, dirs_all, grid_min, grid_max)

        # Accumulate rows in memory-efficient chunks
        out_rows: List[Dict[str, float|int|str]] = []
        for start in tqdm(range(0, n_hits_total, chunk_size), desc=f"Voxel paths ({file_id})", leave=False):
            end = min(start + chunk_size, n_hits_total)
            starts = starts_all[start:end]
            ends   = ends_all[start:end]

            # Centralized FVT traversal for this chunk
            ray_idx, ix, iy, iz, t0, t1, ds = fvt_intersections_basic(
                grid_min, grid_max, grid_shape, starts, ends
            )
            if ray_idx.size == 0:
                continue

            # Convert to rows (offset ray_idx by chunk start → global photon_idx)
            photon_idx = ray_idx + start
            # Assemble as dicts (faster than repeated DataFrame append)
            if include_distance:
                for k in range(len(photon_idx)):
                    row = {
                        'file_id': file_id,
                        'photon_idx': int(photon_idx[k]),
                        'ix': int(ix[k]), 'iy': int(iy[k]), 'iz': int(iz[k]),
                        'dist_mm': float(ds[k]),
                    }
                    if event_col:
                        row['event_id'] = dfh.iloc[int(photon_idx[k])][event_col]
                    out_rows.append(row)
            else:
                for k in range(len(photon_idx)):
                    row = {
                        'file_id': file_id,
                        'photon_idx': int(photon_idx[k]),
                        'ix': int(ix[k]), 'iy': int(iy[k]), 'iz': int(iz[k]),
                    }
                    if event_col:
                        row['event_id'] = dfh.iloc[int(photon_idx[k])][event_col]
                    out_rows.append(row)

            # Optionally: flush periodically if extremely large (omitted; tuples are light)

        # Build a DataFrame once and write to disk
        df_paths = pd.DataFrame(out_rows)
        # Column ordering (consistent/readable)
        lead = ['file_id']
        if event_col:
            lead.append('event_id')
        lead += ['photon_idx', 'ix', 'iy', 'iz']
        if include_distance:
            lead.append('dist_mm')
        df_paths = df_paths[lead]

        save_parquet(df_paths, out_path, metadata=meta)
        LOGGER.debug(f"Saved voxel paths DF: {out_path}")

        del dfh, starts_all, dirs_all, ends_all, df_paths, out_rows
        gc.collect()
        return out_path

    Parallel(n_jobs=num_workers)(
        delayed(_one)(i, *job) for i, job in enumerate(tqdm(jobs, desc="Saving voxel paths"))
    )


# =============================================================================
# Main CLI entry point
# =============================================================================
def main():
    """
    Command-line orchestrator that wires together all of the above pieces:

    • Input: a directory of ROOT files
    • Outputs: user-selected artifacts (DFs, grids, sparse paths, splits, manifest)
    • All paths are fully reproducible and logged verbosely
    """
    parser = argparse.ArgumentParser(description='Generate ML data grids and/or sparse voxel paths with precise progress logs')
    parser.add_argument('input_dir')
    parser.add_argument('output_dir')
    parser.add_argument('-n', '--nFiles', type=int, default=None, help='Max files to process')

    # Geometry & thresholds
    parser.add_argument('--gridSize', type=int, nargs=3, default=[80, 80, 80], help='Grid resolution (nx ny nz)')
    parser.add_argument('--minNHits', type=int, default=0, help='Minimum total hits to keep a file')
    parser.add_argument('--minPrimarySteps', type=int, default=30, help='Minimum primary steps to keep a file')
    parser.add_argument('--primaryPdg', type=int, default=13, help='PDG code to select primaries')

    # Pipeline toggles
    parser.add_argument('--noCuts', action='store_true', help='Skip hits/primary count-based file filtering')
    parser.add_argument('--noCheckFiles', action='store_true', help='Skip histogram shape consistency check')
    parser.add_argument('--useHistograms', action='store_true', help='Build hits DF from histograms instead of trees')

    # Persist options
    parser.add_argument('--saveDFhits', action='store_true', help='Persist hits DataFrames')
    parser.add_argument('--saveDFprimary', action='store_true', help='Persist primary DataFrames')

    parser.add_argument('--saveGridNpyHits', action='store_true', help='Save dense x grids as .npy (reconstruction)')
    parser.add_argument('--saveGridNpyPrimary', action='store_true', help='Save dense y grids as .npy (truth)')
    parser.add_argument('--saveGridH5', action='store_true', help='Save dense x/y grids into one .h5 per file')

    # Sparse voxel paths (fast path using FVT base)
    parser.add_argument('--saveVoxelPaths', action='store_true', help='Save sparse per-photon voxel paths to Parquet')
    parser.add_argument('--voxelPathsUseDistance', action='store_true', help='Include geometric distance per voxel (mm)')
    parser.add_argument('--voxelPathsChunk', type=int, default=20000, help='Chunk size for FVT traversal batching')

    # Split settings (stable hashing; only relevant if H5 grids are produced)
    parser.add_argument('--noSplit', action='store_true', help='Skip writing train/test/val lists')
    parser.add_argument('--pTest', type=float, default=0.20, help='Test fraction (0..1)')
    parser.add_argument('--pVal', type=float, default=0.10, help='Validation fraction (0..1)')
    parser.add_argument('--splitSeed', type=int, default=42, help='Seed used in hashing for splits')
    # Legacy counts (translate to fractions if provided)
    parser.add_argument('--nTest', type=int, default=None, help='(Legacy) Target test size; converted to fraction')
    parser.add_argument('--nVal', type=int, default=None, help='(Legacy) Target val size; converted to fraction')

    # Execution knobs
    parser.add_argument('--numWorkers', type=int, default=1, help='Parallelism level for file-based stages')
    parser.add_argument('--saveAsNumbersOnly-H5', action='store_true', help='Use simple numbered H5 filenames')

    # Orchestration behavior
    parser.add_argument('--force', action='store_true', help='Recompute/overwrite all derived artifacts')
    parser.add_argument('--resume', action='store_true', help='Only compute missing outputs; never overwrite')

    parser.add_argument('-v', '--verbosity', choices=['debug', 'info', 'warning', 'error', 'critical'],
                        default='info')
    args = parser.parse_args()

    # Set up logging and sanity-check geometry
    configure_logging(args.verbosity)
    t_start = time.time()
    LOGGER.info("Starting data generation")
    os.makedirs(args.output_dir, exist_ok=True)
    validate_geometry(tuple(args.gridSize), tuple(DETECTOR_SIZE_MM))

    # Enumerate inputs (optionally limit count)
    files = sorted([f for f in os.listdir(args.input_dir) if f.endswith('.root')])
    if args.nFiles:
        files = files[:args.nFiles]
        LOGGER.info(f"Limiting to first {len(files)} files")
    paths = [os.path.join(args.input_dir, f) for f in files]

    # Optional preflight checks / cuts
    if not args.noCheckFiles:
        paths = check_files(paths, 'photoSensor_hits_histograms', args.numWorkers)
    if not args.noCuts:
        paths = apply_cuts(paths, args.minNHits, args.minPrimarySteps,
                           'photoSensor_hits_histograms', 'primary;1', args.numWorkers)
    if not paths:
        LOGGER.error("No valid input files remain after checks/cuts. Exiting.")
        return

    # Decide whether we need the intermediate DFs (even if not persisted)
    need_h = args.saveDFhits or args.saveGridNpyHits or args.saveGridH5 or args.saveVoxelPaths
    need_p = args.saveDFprimary or args.saveGridNpyPrimary or args.saveGridH5

    dfh_paths: List[str] = []
    dfp_paths: List[str] = []
    manifest: List[Dict[str, Optional[str]]] = []

    # Build dataframes, some persisted, some in a temp folder
    import tempfile
    with tempfile.TemporaryDirectory() as tmp:
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
                                           pb_base, args.primaryPdg,
                                           args.numWorkers, force=args.force, resume=args.resume)

        # Prepare filenames for dense grids
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

        # Dense grids (optional)
        if args.saveGridNpyHits or args.saveGridNpyPrimary or args.saveGridH5:
            if not (dfh_paths and dfp_paths):
                LOGGER.warning("Grid creation requested but hits/primary DFs are missing. Skipping grids.")
            else:
                create_grids(dfh_paths, dfp_paths,
                             npy_h, npy_p, h5s,
                             args.saveGridNpyHits, args.saveGridNpyPrimary,
                             args.saveGridH5,
                             gs, combine_walls=True,
                             walls_combine=False,
                             walls_method=expNWalls,
                             combine_vectors=True,
                             num_workers=args.numWorkers, force=args.force, resume=args.resume)

        # Sparse voxel paths (fast; recommended)
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
                             chunk_size=args.voxelPathsChunk)

        # Train/test/val split (only relevant if H5 grids produced)
        if not args.noSplit and args.saveGridH5:
            if len(h5s) == 0:
                LOGGER.warning("Requested dataset split, but no H5 grids were created. Skipping split.")
            else:
                p_test = float(args.pTest)
                p_val = float(args.pVal)
                # Translate legacy absolute counts to fractions when provided
                if args.nTest is not None and len(h5s) > 0:
                    p_test = max(0.0, min(1.0, args.nTest / float(len(h5s))))
                    LOGGER.info(f"Using legacy nTest={args.nTest} → pTest≈{p_test:.3f}")
                if args.nVal is not None and len(h5s) > 0:
                    p_val = max(0.0, min(1.0, args.nVal / float(len(h5s))))
                    LOGGER.info(f"Using legacy nVal={args.nVal} → pVal≈{p_val:.3f}")
                if p_test + p_val >= 1.0:
                    raise ValueError(f"pTest + pVal must be < 1.0 (got {p_test+p_val})")

                train, test, val = split_data_stable(h5s, p_test=p_test, p_val=p_val, seed=args.splitSeed)
                for name, lst in [('train', train), ('test', test), ('val', val)]:
                    of = os.path.join(args.output_dir, f"{name}_paths.txt")
                    with open(of, 'w') as f:
                        f.write("\n".join(lst))
                    LOGGER.info(f"Wrote {name} paths: {of}")

        # Build a manifest row per *input file* (post-cuts)
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

    # Persist manifest to the output directory
    manifest_path = os.path.join(args.output_dir, "manifest.json")
    with open(manifest_path, "w") as f:
        json.dump(manifest, f, indent=2)
    LOGGER.info(f"Wrote manifest: {manifest_path}")

    # Summary
    dt = time.time() - t_start
    n_dfh = sum(1 for m in manifest if m["df_hits"])
    n_dfp = sum(1 for m in manifest if m["df_primary"])
    n_h5  = sum(1 for m in manifest if m["grid_h5"])
    n_vp  = sum(1 for m in manifest if m["voxel_paths"])
    LOGGER.info(f"Completed in {dt:.1f}s — hitsDF={n_dfh}, primaryDF={n_dfp}, H5={n_h5}, voxelPaths={n_vp}")


# =============================================================================
# Entrypoint
# =============================================================================
if __name__ == '__main__':
    main()
