# gridMethods.py
# =============================================================================
# High-level utilities for building voxel grids and related helpers
# =============================================================================
# This module wraps a fast voxel traversal (FVT) backend to:
#   * enumerate which voxels a straight segment passes through (with entry/exit
#     parameters and in-voxel distances),
#   * aggregate those per-segment traversals into dense 3D grids,
#   * optionally return per-segment (“per-hit”) dense grids,
#   * support distance-weighted accumulation and wall-aware accumulation,
#   * provide coordinate/index conversion helpers and simple error spreading.
#
# The centerpiece of this refactor is `fvt_intersections_basic(...)`, a small,
# “bare-bones” function that calls directly into the FVT package and returns,
# for a list of segments, the list of traversed voxel indices along with the
# local parametric entry/exit values and physical distances. All higher-level
# grid builders call through this one place so traversal logic remains in
# exactly one spot.
# =============================================================================

from __future__ import annotations

from collections import defaultdict
from typing import Callable, Dict, Iterable, List, Optional, Tuple

import numpy as np
from scipy.spatial.distance import cdist

from constants import *  # expects DETECTOR_SIZE_MM and any other geometry constants
import fast_voxel_traversal as fvt  # backend used to enumerate voxel crossings


# =============================================================================
# Basic FVT wrapper (single source of truth for traversal)
# =============================================================================
def fvt_intersections_basic(
    grid_minBound: np.ndarray,
    grid_maxBound: np.ndarray,
    grid_shape: np.ndarray,
    vector_starts: np.ndarray,
    vector_ends: np.ndarray,
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    """
    Run the lowest-level "fast voxel traversal" over a batch of line segments.

    This function does the minimal amount of work necessary:
      1) Build a single FVT grid object from the provided bounds/shape.
      2) For each input segment i, compute its direction d_i = end_i - start_i.
      3) Ask FVT to enumerate which voxels are crossed and at what local
         parameters (t_enter, t_exit) along the *finite* segment.
      4) Clamp those local parameters to [0, 1] and compute the physical
         distance spent inside each voxel: ds = (t_exit - t_enter) * ||d_i||.
      5) Append one output row per voxel crossing.

    Parameters
    ----------
    grid_minBound, grid_maxBound : (3,) float array
        Axis-aligned box bounds for the voxel grid. Coordinates are in *mm*.
        Commonly (-det/2, +det/2) if the detector is centered at the origin.
    grid_shape : (3,) int array
        Number of voxels along each axis: (nx, ny, nz).
    vector_starts, vector_ends : (N, 3) float arrays
        Finite segment endpoints in mm. If a segment lies fully outside and does
        not intersect the box, it simply contributes zero rows.

    Returns
    -------
    ray_index  : (M,) int
        For each voxel crossing row (M total), the index i in [0..N) of the
        input segment that produced it.
    ix, iy, iz : (M,) int
        Integer voxel indices in the grid (0-based along x,y,z).
    t_enter_u  : (M,) float
        Local parametric entry value in [0, 1] along the *finite* segment
        start→end. 0 means the segment start; 1 means the segment end.
    t_exit_u   : (M,) float
        Local parametric exit value in [0, 1] along the *finite* segment.
        Always >= t_enter_u for valid rows.
    dist_mm    : (M,) float
        Physical distance in millimeters the segment travels inside this voxel
        (equal to (t_exit_u - t_enter_u) * ||end - start||).

    Notes
    -----
    * No weighting happens here. This is purely geometric enumeration + ds.
    * All higher-level functions (dense grid accumulation, per-hit grids, wall
      handling) should build on top of this to ensure identical traversal.
    """
    # Normalize/validate inputs
    grid_minBound = np.asarray(grid_minBound, dtype=float).reshape(3)
    grid_maxBound = np.asarray(grid_maxBound, dtype=float).reshape(3)
    grid_shape    = np.asarray(grid_shape, dtype=int   ).reshape(3)

    vector_starts = np.asarray(vector_starts, dtype=float).reshape(-1, 3)
    vector_ends   = np.asarray(vector_ends,   dtype=float).reshape(-1, 3)
    assert vector_starts.shape == vector_ends.shape, "starts and ends must be (N,3)"

    # Build traversal grid once (fast reuse for many rays)
    voxel_size = (grid_maxBound - grid_minBound) / grid_shape
    FVTgrid = fvt.Grid(grid_shape=grid_shape, voxel_size=voxel_size, grid_origin=grid_minBound)

    # Output containers (append lists → stack once; faster than frequent realloc)
    rows_ray  : List[int]   = []
    rows_ix   : List[int]   = []
    rows_iy   : List[int]   = []
    rows_iz   : List[int]   = []
    rows_t0   : List[float] = []
    rows_t1   : List[float] = []
    rows_ds   : List[float] = []

    # Enumerate voxel crossings ray-by-ray
    for i in range(len(vector_starts)):
        start = vector_starts[i]
        end   = vector_ends[i]

        # Direction of the finite segment and its length (for geometric distance)
        direction = end - start
        dir_len = float(np.linalg.norm(direction))
        if not np.isfinite(dir_len) or dir_len == 0.0:
            continue  # degenerate; contributes nothing

        # FVT yields (ix,iy,iz,t_enter,t_exit) with t defined along the vector
        # we pass in. We clamp that local parameter to [0,1] so we only retain
        # the portion of the finite segment inside the voxel.
        for ix, iy, iz, t_enter, t_exit in FVTgrid.traverse(start, direction):
            t0 = max(float(t_enter), 0.0)
            t1 = min(float(t_exit),  1.0)
            if t1 <= t0:
                continue  # empty (just grazes a face or outside the 0..1 span)

            ds = (t1 - t0) * dir_len  # physical distance traveled in this voxel (mm)

            rows_ray.append(i)
            rows_ix .append(int(ix))
            rows_iy .append(int(iy))
            rows_iz .append(int(iz))
            rows_t0 .append(t0)
            rows_t1 .append(t1)
            rows_ds .append(ds)

    # Stack to NumPy for efficient downstream accumulation
    if not rows_ray:
        # Return empty arrays with correct dtypes if nothing intersected
        return (
            np.array([], dtype=int),  # ray_index
            np.array([], dtype=int),  # ix
            np.array([], dtype=int),  # iy
            np.array([], dtype=int),  # iz
            np.array([], dtype=float),  # t_enter_u
            np.array([], dtype=float),  # t_exit_u
            np.array([], dtype=float),  # dist_mm
        )

    return (
        np.asarray(rows_ray, dtype=int),
        np.asarray(rows_ix,  dtype=int),
        np.asarray(rows_iy,  dtype=int),
        np.asarray(rows_iz,  dtype=int),
        np.asarray(rows_t0,  dtype=float),
        np.asarray(rows_t1,  dtype=float),
        np.asarray(rows_ds,  dtype=float),
    )


# =============================================================================
# Grid builders (implemented on top of the basic traversal)
# =============================================================================
def get_voxelGrid_hitVector(
    grid_minBound: np.ndarray,
    grid_maxBound: np.ndarray,
    grid_shape:   np.ndarray, 
    vector_starts: np.ndarray,
    vector_ends:   np.ndarray,
    vector_weights: Optional[np.ndarray] = None,
    use_distance: bool = False,
    return_individual: bool = False,
    return_distances: bool = False,
):
    """
    Accumulate a dense 3D grid from many finite segments using the basic FVT traversal.

    This function now delegates all voxel enumeration to `fvt_intersections_basic(...)`
    and focuses solely on *accumulation policy*:
      * When `use_distance=False`: each voxel crossing contributes `weight_i` once.
      * When `use_distance=True` : each crossing contributes `weight_i * ds_mm`.

    Optionally, it also returns:
      * `grids_individual`: a dense per-hit grid (one grid per input segment),
      * `grid_distance`   : the cumulative *unweighted* ds per voxel (pure geometry),
      * `grids_distance_individual`: per-hit ds grids.

    Parameters
    ----------
    grid_minBound, grid_maxBound, grid_shape
        Voxel grid axis-aligned bounds and integer shape (see basic function).
    vector_starts, vector_ends
        Segment endpoints in mm (shape (N,3)).
    vector_weights
        Optional per-segment weights w_i (shape (N,)). Defaults to 1.
    use_distance
        If True, weight each crossing by distance (w_i * ds). If False, just w_i.
    return_individual
        If True, also return an array shaped (N, *grid_shape) with per-hit *weighted*
        grids (memory heavy for large N or high resolution).
    return_distances
        If True, return distance-only grids (cumulative and, if requested, per-hit).

    Returns
    -------
    grid : (nx,ny,nz) float
        Cumulative *weighted* grid.
    grids_individual : (N,nx,ny,nz) float, optional
        Returned only if `return_individual=True`.
    grid_distance : (nx,ny,nz) float, optional
        Returned only if `return_distances=True`. Pure distances ds per voxel (no w_i).
    grids_distance_individual : (N,nx,ny,nz) float, optional
        Returned only if both `return_distances` and `return_individual` are True.

    Notes
    -----
    * All accumulation now happens with `np.add.at`, which is order-independent
      and safe for repeated indices. This matches the previous behavior while
      centralizing the traversal.
    * Per-hit grids are expensive to materialize; prefer downstream sparse usage
      if you only need coordinates/distances.
    """
    # Normalize inputs
    grid_minBound = np.asarray(grid_minBound, dtype=float).reshape(3)
    grid_maxBound = np.asarray(grid_maxBound, dtype=float).reshape(3)
    grid_shape    = np.asarray(grid_shape,   dtype=int  ).reshape(3)
    vector_starts = np.asarray(vector_starts, dtype=float).reshape(-1, 3)
    vector_ends   = np.asarray(vector_ends,   dtype=float).reshape(-1, 3)

    N = vector_starts.shape[0]
    if vector_weights is None:
        vector_weights = np.ones(N, dtype=float)
    else:
        vector_weights = np.asarray(vector_weights, dtype=float).reshape(N)

    # === Core traversal (single source of truth) ===
    ray_idx, ix, iy, iz, t_enter_u, t_exit_u, dist_mm = fvt_intersections_basic(
        grid_minBound, grid_maxBound, grid_shape, vector_starts, vector_ends
    )

    # Early-out for empty intersections (still return requested shapes)
    grid = np.zeros(tuple(grid_shape), dtype=float)
    want_individual = bool(return_individual)
    want_distances  = bool(return_distances)
    grids_individual = None
    grids_distance_individual = None
    grid_distance = None

    if ray_idx.size == 0:
        # If the user asked for the extra products, return zero-filled arrays with
        # the right shapes for consistency.
        if want_individual:
            grids_individual = np.zeros((N,) + tuple(grid_shape), dtype=float)
        if want_distances:
            grid_distance = np.zeros(tuple(grid_shape), dtype=float)
            if want_individual:
                grids_distance_individual = np.zeros((N,) + tuple(grid_shape), dtype=float)

        # Package results exactly like before
        if want_distances and want_individual:
            return grid, grids_individual, grid_distance, grids_distance_individual
        if want_distances:
            return grid, grid_distance
        if want_individual:
            return grid, grids_individual
        return grid

    # === Weighted accumulation (global/cumulative) ===
    # Per-row weight: either w_i or w_i * ds depending on `use_distance`.
    row_weights = vector_weights[ray_idx]
    if use_distance:
        row_weights = row_weights * dist_mm  # scale by physical path length in the voxel

    # Accumulate into the global grid using fancy indexing
    np.add.at(grid, (ix, iy, iz), row_weights)

    # Distance-only cumulative grid if requested
    if want_distances:
        grid_distance = np.zeros_like(grid)
        np.add.at(grid_distance, (ix, iy, iz), dist_mm)

    # === Per-hit grids (optional, memory heavy) ===
    if want_individual:
        grids_individual = np.zeros((N,) + tuple(grid_shape), dtype=float)
        if want_distances:
            grids_distance_individual = np.zeros((N,) + tuple(grid_shape), dtype=float)

        # For each ray i, gather its rows and add to the i-th sub-grid
        # This keeps behavior identical to the previous implementation while
        # avoiding double traversal.
        for i in range(N):
            m = (ray_idx == i)
            if not np.any(m):
                continue
            if use_distance:
                w_i = vector_weights[i] * dist_mm[m]
            else:
                # Each voxel crossing contributes weight_i once
                w_i = np.full(m.sum(), vector_weights[i], dtype=float)
            np.add.at(grids_individual[i], (ix[m], iy[m], iz[m]), w_i)

            if want_distances:
                np.add.at(grids_distance_individual[i], (ix[m], iy[m], iz[m]), dist_mm[m])

    # === Return with exactly the same arity as before ===
    if want_distances and want_individual:
        return grid, grids_individual, grid_distance, grids_distance_individual
    if want_distances:
        return grid, grid_distance
    if want_individual:
        return grid, grids_individual
    return grid


def get_voxelGrid(
    grid_shape: np.ndarray, 
    grid_dimensions: np.ndarray, 
    vector_starts: np.ndarray,
    vector_directions: np.ndarray,
    vector_weights: Optional[np.ndarray] = None,
    vector_start_walls: Optional[np.ndarray] = None,
    walls: bool = False,
    walls_combine: bool = False,
    walls_combine_method: Optional[Callable] = None,
    vector_combine: bool = False,
    use_distance: bool = False,
    return_individual: bool = False,
    return_distances: bool = False
):
    """
    Public grid builder that mirrors previous behavior but now delegates voxel
    enumeration to `fvt_intersections_basic(...)` via `get_voxelGrid_hitVector(...)`.

    Parameters (summarized)
    -----------------------
    grid_shape : (3,) int
        Integer voxel counts along x,y,z.
    grid_dimensions : (3,) float
        Physical detector size in mm along x,y,z. Bounds are taken as [-D/2, +D/2].
    vector_starts : (N,3) float
        Segment start points (e.g., sensor positions).
    vector_directions : (N,3) float
        Segment directions (e.g., negative reconstructed directions).
        This is converted to finite `vector_ends` inside the detector box.
    vector_weights : (N,) float, optional
        Per-hit weights (defaults to 1).
    vector_start_walls : (N,) int, optional
        Encoded wall indices (see `wallStringToInt`) used when `walls=True`.
    walls, walls_combine, walls_combine_method
        Wall-aware accumulation options (unchanged semantics).
    vector_combine : bool
        If True, combine duplicates (same start/end cell indices) before
        accumulation to reduce traversal volume (unchanged semantics).
    use_distance, return_individual, return_distances
        Accumulation/return policy (see `get_voxelGrid_hitVector`).

    Returns
    -------
    Same return structure as before. When `walls=False`, returns the direct
    outputs of `get_voxelGrid_hitVector`. When `walls=True`, returns either a
    combined grid or a stacked grid, plus optional per-wall products.
    """
    # Normalize arrays and relax overly strict dtype assertions
    grid_shape = np.asarray(grid_shape, dtype=int).reshape(3)
    grid_dimensions = np.asarray(grid_dimensions, dtype=float).reshape(3)

    if vector_weights is None:
        vector_weights = np.ones(len(vector_starts), dtype=float)

    assert vector_starts.shape == vector_directions.shape and vector_starts.shape[1] == 3
    if walls:
        assert vector_start_walls is not None, "walls=True requires vector_start_walls"
        if walls_combine:
            assert walls_combine_method is not None, "walls_combine=True requires walls_combine_method"

    walls_intOptions = wallStringToInt(np.array(['+x', '-x', '+y', '-y', '+z', '-z']))

    grid_bounds_min = grid_dimensions / -2.0
    grid_bounds_max = grid_dimensions /  2.0

    # Clip directions to finite segment ends at the detector bounds
    vector_starts = np.asarray(vector_starts, dtype=float).reshape(-1, 3)
    vector_directions = np.asarray(vector_directions, dtype=float).reshape(-1, 3)
    vector_ends = get_vectors_end(vector_starts, vector_directions, grid_bounds_min, grid_bounds_max)

    # Optionally combine identical segments (by start/end *index*, not position)
    if vector_combine:
        vector_starts_ind = get_indexFromPosition(vector_starts, grid_bounds_min, grid_bounds_max, grid_shape)
        vector_ends_ind   = get_indexFromPosition(vector_ends,   grid_bounds_min, grid_bounds_max, grid_shape)

        vector_starts_ind, vector_ends_ind, vector_weights, inds = get_combinedVectors(
            vector_starts_ind, vector_ends_ind, vector_weights
        )

        vector_starts = get_positionFromIndex(vector_starts_ind, grid_bounds_min, grid_bounds_max, grid_shape)
        vector_ends   = get_positionFromIndex(vector_ends_ind,   grid_bounds_min, grid_bounds_max, grid_shape)

        if walls and vector_start_walls is not None:
            vector_start_walls = vector_start_walls[inds]

    # ---- No walls: one call and done
    if not walls:
        return get_voxelGrid_hitVector(
            grid_bounds_min, grid_bounds_max, grid_shape,
            vector_starts, vector_ends, vector_weights,
            use_distance=use_distance,
            return_individual=return_individual,
            return_distances=return_distances,
        )

    # ---- With walls: split, process, then combine/stack like before
    wall_indices: Dict[int, np.ndarray] = {wall: np.flatnonzero(vector_start_walls == wall) for wall in walls_intOptions}

    grid_walls = {}
    grids_individual_walls = {} if return_individual else None
    grid_distance_walls = {} if return_distances else None
    grids_distance_individual_walls = {} if (return_individual and return_distances) else None

    for wall in walls_intOptions:
        if wall_indices[wall].size == 0:
            continue

        sel = wall_indices[wall]
        wall_starts  = vector_starts [sel]
        wall_ends    = vector_ends   [sel]
        wall_weights = vector_weights[sel]

        wall_result = get_voxelGrid_hitVector(
            grid_bounds_min, grid_bounds_max, grid_shape,
            wall_starts, wall_ends, wall_weights,
            use_distance=use_distance,
            return_individual=return_individual,
            return_distances=return_distances,
        )

        # Unpack with the same arity as the called function
        if return_distances and return_individual:
            g, gi, gd, gdi = wall_result
            grid_walls[wall] = g
            grids_individual_walls[wall] = gi
            grid_distance_walls[wall] = gd
            grids_distance_individual_walls[wall] = gdi
        elif return_distances:
            g, gd = wall_result
            grid_walls[wall] = g
            grid_distance_walls[wall] = gd
        elif return_individual:
            g, gi = wall_result
            grid_walls[wall] = g
            grids_individual_walls[wall] = gi
        else:
            g = wall_result  # only the cumulative grid
            grid_walls[wall] = g

    if walls_combine:
        # Combine cumulative weighted grids using the provided reducer
        grid_combined = walls_combine_method(list(grid_walls.values()))
        returns: List[np.ndarray] = [grid_combined]

        if return_individual:
            # Sum per-hit weighted grids across walls. Because different walls may
            # carry different numbers of hits, align by per-wall hit axis length.
            max_hits = max((arr.shape[0] for arr in grids_individual_walls.values()), default=0)
            gi_sum = np.zeros((max_hits,) + tuple(grid_shape), dtype=float)
            for gi in grids_individual_walls.values():
                if gi.shape[0] < max_hits:
                    pad = np.zeros((max_hits - gi.shape[0],) + gi.shape[1:], dtype=gi.dtype)
                    gi = np.concatenate([gi, pad], axis=0)
                gi_sum += gi
            returns.append(gi_sum)

        if return_distances:
            gd_sum = np.zeros(tuple(grid_shape), dtype=float)
            for gd in grid_distance_walls.values():
                gd_sum += gd
            returns.append(gd_sum)

            if return_individual:
                max_hits = max((arr.shape[0] for arr in grids_distance_individual_walls.values()), default=0)
                gdi_sum = np.zeros((max_hits,) + tuple(grid_shape), dtype=float)
                for gdi in grids_distance_individual_walls.values():
                    if gdi.shape[0] < max_hits:
                        pad = np.zeros((max_hits - gdi.shape[0],) + gdi.shape[1:], dtype=gdi.dtype)
                        gdi = np.concatenate([gdi, pad], axis=0)
                    gdi_sum += gdi
                returns.append(gdi_sum)

        return tuple(returns) if len(returns) > 1 else returns[0]

    # walls_combine == False → return a stacked grid (last axis = 6 walls)
    grid_stacked = np.zeros([*grid_shape, 6], dtype=float)
    for wall in walls_intOptions:
        if wall in grid_walls:
            grid_stacked[:, :, :, wall] = grid_walls[wall]

    returns = [grid_stacked]
    if return_individual:
        returns.append(grids_individual_walls)
    if return_distances:
        returns.append(grid_distance_walls)
        if return_individual:
            returns.append(grids_distance_individual_walls)

    return tuple(returns) if len(returns) > 1 else returns[0]


# =============================================================================
# Ancillary utilities (documented and unchanged in behavior)
# =============================================================================
def get_voxelGrid_errors(grid: np.ndarray) -> np.ndarray:
    """
    Fill empty voxels with an exponentially decayed value from their nearest hit.

    This is a simple spatial "smoothing"/imputation that:
      * finds all zero voxels and all non-zero voxels,
      * computes Euclidean distance from each zero voxel to its nearest hit,
      * assigns p = exp(-distance) * (value at nearest hit).

    Parameters
    ----------
    grid : (nx,ny,nz) float
        Input grid (typically counts or weights).

    Returns
    -------
    grid_copy : (nx,ny,nz) float
        A *new* grid with zeros replaced; the original is not modified.
    """
    grid_copy = grid.copy()

    zeros = np.argwhere(grid_copy == 0)
    hit = np.argwhere(grid_copy > 0)
    if len(hit) == 0:
        return grid_copy

    distances = cdist(zeros, hit, metric='euclidean')
    closestHit = np.argmin(distances, axis=1)
    distances = distances[np.arange(len(distances)), closestHit]
    closestHit_ind = hit[closestHit]
    hitValues = grid_copy[closestHit_ind[:, 0], closestHit_ind[:, 1], closestHit_ind[:, 2]]
    p = np.exp(-distances)
    grid_copy[zeros[:, 0], zeros[:, 1], zeros[:, 2]] = p * hitValues

    return grid_copy


def get_voxelGrid_ind(size: Iterable[int]) -> np.ndarray:
    """
    Return all voxel indices as a (nx*ny*nz, 3) array of integer coordinates.

    Example
    -------
    >>> get_voxelGrid_ind((2,2,2))
    array([[0,0,0],[0,0,1],[0,1,0],[0,1,1],[1,0,0],[1,0,1],[1,1,0],[1,1,1]])
    """
    size = np.asarray(size, dtype=int).reshape(3)
    grid_ind = np.indices(size).reshape(3, -1).T
    return grid_ind


def get_voxelGrid_pos(size: Iterable[int], detectorDimensions: Iterable[float]) -> np.ndarray:
    """
    Map each voxel index to its *minimum-corner* position in mm in detector coords.

    Uses a uniform scale: size / detectorDimensions, and assumes the detector box
    spans [-D/2, +D/2] along each axis.

    Returns
    -------
    (nx*ny*nz, 3) float array of the lower-corner positions per voxel.
    """
    size = np.asarray(size, dtype=int).reshape(3)
    detectorDimensions = np.asarray(detectorDimensions, dtype=float).reshape(3)

    scale = size / detectorDimensions
    grid_ind = get_voxelGrid_ind(size)
    grid_pos = grid_ind / scale - detectorDimensions[np.newaxis, :] / 2.0

    return grid_pos


def get_voxelGrid_centers(size: Iterable[int], detectorDimensions: Iterable[float]) -> np.ndarray:
    """
    Compute the *center* position of every voxel, returned as a dense 4D array.

    Parameters
    ----------
    size : (3,) int
        Grid shape (nx, ny, nz).
    detectorDimensions : (3,) float
        Physical detector size in mm (Dx, Dy, Dz). Assumed box bounds [-D/2, +D/2].

    Returns
    -------
    centers : (nx,ny,nz,3) float
        Per-voxel center coordinates (x,y,z) in mm.

    Notes
    -----
    This routine is a convenience helper for visualization/meshing. For bulk
    processing prefer the index→position conversions for memory efficiency.
    """
    size = np.asarray(size, dtype=int).reshape(3)
    detectorDimensions = np.asarray(detectorDimensions, dtype=float).reshape(3)

    x0_grid = -detectorDimensions[0] / 2.0
    y0_grid = -detectorDimensions[1] / 2.0
    z0_grid = -detectorDimensions[2] / 2.0

    x_centers = x0_grid + (np.arange(size[0]) + 0.5) * detectorDimensions[0] / size[0]
    y_centers = y0_grid + (np.arange(size[1]) + 0.5) * detectorDimensions[1] / size[1]
    z_centers = z0_grid + (np.arange(size[2]) + 0.5) * detectorDimensions[2] / size[2]

    centers = np.zeros(size.tolist() + [3], dtype=np.float64)
    centers[:, :, :, 0] = x_centers[:, None, None]
    centers[:, :, :, 1] = y_centers[None, :, None]
    centers[:, :, :, 2] = z_centers[None, None, :]

    return centers


def get_vectors_end(
    vector_starts: np.ndarray,
    vector_directions: np.ndarray,
    grid_minBound: np.ndarray,
    grid_maxBound: np.ndarray,
    epsilon: float = 1e-6
) -> np.ndarray:
    """
    Compute finite segment end points by intersecting infinite rays with the grid box.

    For each start + t*direction, we compute the maximum among axis-wise entry
    parameters and the minimum among exits. The resulting t_end defines a point
    on the ray that lies just inside the box (we clamp by a tiny epsilon to
    avoid landing exactly on the max bound).

    Returns
    -------
    vector_ends : (N,3) float
        End points in mm; NaNs are not returned — out-of-box rays are clamped so
        all outputs lie in [grid_minBound, grid_maxBound).
    """
    assert vector_starts.shape == vector_directions.shape
    assert grid_minBound.shape == grid_maxBound.shape == (3,)

    t_min = np.where(vector_directions != 0, (grid_minBound - vector_starts) / vector_directions, -np.inf)
    t_max = np.where(vector_directions != 0, (grid_maxBound - vector_starts) / vector_directions,  np.inf)

    # Axis-wise entry/exit params, then the slab intersection
    t_enter = np.minimum(t_min, t_max)
    t_exit  = np.maximum(t_min, t_max)

    t_start = np.max(t_enter, axis=1)
    t_end   = np.min(t_exit,  axis=1)

    # Keep the segment inside the box; rays that miss will end up with t_end<=t_start,
    # which will lead to zero traversal in downstream code.
    t_positive = t_end > t_start
    valid_t = np.where(t_positive, t_start, np.nan)

    vector_ends = vector_starts + np.expand_dims(valid_t, axis=1) * vector_directions

    # Clamp exactly at the upper bounds by epsilon to preserve index validity
    vector_ends = np.where(vector_ends >= grid_maxBound, grid_maxBound - epsilon, vector_ends)
    assert (grid_minBound <= vector_ends).all()
    assert (vector_ends <  grid_maxBound).all()

    return vector_ends


def get_indexFromPosition(
    position: np.ndarray,
    grid_minBound: np.ndarray,
    grid_maxBound: np.ndarray,
    grid_shape: np.ndarray
) -> np.ndarray:
    """
    Convert physical positions (mm) to integer voxel indices.

    Parameters
    ----------
    position : (N,3) float
        Positions to index (must lie within [minBound, maxBound) ).
    grid_minBound, grid_maxBound : (3,) float
        Grid AABB bounds in mm.
    grid_shape : (3,) int
        Grid shape (nx, ny, nz).

    Returns
    -------
    indices : (N,3) int
        Corresponding voxel indices along x,y,z.
    """
    position = np.asarray(position, dtype=float).reshape(-1, 3)
    grid_minBound = np.asarray(grid_minBound, dtype=float).reshape(3)
    grid_maxBound = np.asarray(grid_maxBound, dtype=float).reshape(3)
    grid_shape    = np.asarray(grid_shape,    dtype=int  ).reshape(3)

    assert (grid_minBound <= position).all() and (position < grid_maxBound).all(), \
        "Positions must lie inside [grid_minBound, grid_maxBound)."

    scale = (grid_maxBound - grid_minBound) / grid_shape
    indices = ((position - grid_minBound) / scale).astype(int)

    assert (0 <= indices).all() and (indices < grid_shape).all()
    return indices


def get_positionFromIndex(
    index: np.ndarray,
    grid_minBound: np.ndarray,
    grid_maxBound: np.ndarray,
    grid_shape: np.ndarray
) -> np.ndarray:
    """
    Convert voxel indices (ix,iy,iz) to physical *minimum-corner* positions in mm.

    Parameters
    ----------
    index : (N,3) int
        Voxel indices along x,y,z.
    grid_minBound, grid_maxBound : (3,) float
        Grid AABB bounds in mm.
    grid_shape : (3,) int
        Grid shape (nx, ny, nz).

    Returns
    -------
    positions : (N,3) float
        Lower-corner physical positions for each index.
    """
    index = np.asarray(index, dtype=int).reshape(-1, 3)
    grid_minBound = np.asarray(grid_minBound, dtype=float).reshape(3)
    grid_maxBound = np.asarray(grid_maxBound, dtype=float).reshape(3)
    grid_shape    = np.asarray(grid_shape,    dtype=int  ).reshape(3)

    assert (0 <= index).all() and (index < grid_shape).all(), "Indices out of range."

    scale = (grid_maxBound - grid_minBound) / grid_shape
    positions = grid_minBound + index * scale
    return positions


def wallStringToInt(walls_str: np.ndarray) -> np.ndarray:
    """
    Map wall strings ('+x','-x','+y','-y','+z','-z') to integer codes [0..5].

    Parameters
    ----------
    walls_str : (N,) array-like of str
        Wall labels per segment.

    Returns
    -------
    walls_int : (N,) int
        Encoded wall indices: +x→0, -x→1, +y→2, -y→3, +z→4, -z→5, else -1.
    """
    walls_str = np.asarray(walls_str, dtype=object).reshape(-1)
    walls_int = np.full(len(walls_str), -1, dtype=int)

    for i in range(len(walls_str)):
        wall = walls_str[i]
        if   wall == '+x': walls_int[i] = 0
        elif wall == '-x': walls_int[i] = 1
        elif wall == '+y': walls_int[i] = 2
        elif wall == '-y': walls_int[i] = 3
        elif wall == '+z': walls_int[i] = 4
        elif wall == '-z': walls_int[i] = 5

    return walls_int


def get_combinedVectors(
    vector_starts_ind: np.ndarray,
    vector_ends_ind:   np.ndarray,
    vector_weights: Optional[np.ndarray] = None
) -> Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]:
    """
    Combine duplicate segments that share identical (start_index, end_index).

    This is a simple reduction that:
      * sums weights across duplicates,
      * returns the index of the *first* occurrence for back-references (e.g.,
        to keep aligned arrays like wall codes).

    Parameters
    ----------
    vector_starts_ind, vector_ends_ind : (N,3) int
        Voxel indices (not positions) of the segment start/end.
    vector_weights : (N,) float
        Optional weights (defaults to ones).

    Returns
    -------
    combined_starts_ind : (M,3) int
    combined_ends_ind   : (M,3) int
    combined_weights    : (M,)   float
    combined_indices    : (M,)   int
        Index of a representative original segment for each combined one.
    """
    if vector_weights is None:
        vector_weights = np.ones(len(vector_starts_ind), dtype=float)

    assert vector_starts_ind.shape == vector_ends_ind.shape == (len(vector_weights), 3)
    assert vector_weights.shape == (len(vector_weights),)

    # Accumulate weights by key (start,end) and keep the first index
    vector_dict: Dict[Tuple[Tuple[int,int,int], Tuple[int,int,int]], Dict[str, float|int]] = \
        defaultdict(lambda: {"weight": 0.0, "index": -1})

    for i, (start, end, weight) in enumerate(zip(map(tuple, vector_starts_ind),
                                                 map(tuple, vector_ends_ind),
                                                 vector_weights)):
        vector_dict[(start, end)]["weight"] += float(weight)
        if vector_dict[(start, end)]["index"] == -1:
            vector_dict[(start, end)]["index"] = i

    combined_pairs = list(vector_dict.keys())
    combined_starts_ind = np.array([p[0] for p in combined_pairs], dtype=int)
    combined_ends_ind   = np.array([p[1] for p in combined_pairs], dtype=int)
    combined_weights    = np.array([d["weight"] for d in vector_dict.values()], dtype=float)
    combined_indices    = np.array([d["index"]  for d in vector_dict.values()], dtype=int)

    return combined_starts_ind, combined_ends_ind, combined_weights, combined_indices


# =============================================================================
# Wall-combining helper (unchanged semantics; documented)
# =============================================================================
def expNWalls(wallGrids: List[np.ndarray], walls: Iterable[int], nWalls: int = 6, returnWalls: bool = False):
    """
    Combine per-wall grids with a heuristic that penalizes voxels hit by fewer walls.

    The idea: voxels activated by multiple distinct entry walls are retained
    (or emphasized); others are suppressed. The current implementation:
      * builds a boolean mask per wall for voxels with >0 value,
      * counts how many walls hit each voxel,
      * keeps voxels with 2+ walls (copy their summed value into the final grid),
      * optionally returns a grid that carries the per-wall values where the
        final grid is non-zero.

    Parameters
    ----------
    wallGrids : list of (nx,ny,nz) arrays
        Per-wall cumulative grids.
    walls : iterable of wall ids
        Order must correspond to wallGrids.
    nWalls : int
        Expected total number of walls (6 for a box).
    returnWalls : bool
        If True, also return the per-wall values restricted to the final support.

    Returns
    -------
    grid_final : (nx,ny,nz) float
        Combined grid.
    grid_finalWithWalls : (nx,ny,nz,nWalls) float, optional
        Returned only if returnWalls=True.
    """
    grid_sum = np.sum(wallGrids, axis=0)
    grid_walls = np.full(grid_sum.shape + (nWalls,), False, dtype=bool)

    for i, wall in enumerate(walls):
        grid_walls[wallGrids[i] > 0, i] = True

    grid_nWalls = np.sum(grid_walls, axis=3)
    for flat in np.where(grid_nWalls == 2)[0]:
        idx = np.unravel_index(flat, grid_nWalls.shape)
        ind = grid_walls[idx]
        # If exactly two are true and they are opposite faces, reduce to 1
        if (ind[0] and ind[1]) or (ind[2] and ind[3]) or (ind[4] and ind[5]):
            grid_nWalls[idx] = 1

    grid_final = np.zeros(grid_sum.shape, dtype=float)
    grid_final[grid_nWalls > 1] = grid_sum[grid_nWalls > 1]

    if not returnWalls:
        return grid_final
    
    grid_finalWithWalls = np.zeros(grid_sum.shape + (nWalls,))
    for i in range(nWalls):
        grid_finalWithWalls[:, :, :, i] = np.where(grid_final <= 0, 0, wallGrids[i])

    return grid_finalWithWalls


# =============================================================================
# Time-bin convenience wrapper (unchanged semantics; documented)
# =============================================================================
def make_voxelGrids_withTimeSteps(
    df_hits,
    nTimeBins: int, 
    shape: Tuple[int,int,int] = (10,10,10),
    detectorDimensions: Iterable[float] = DETECTOR_SIZE_MM,
    make_errors: bool = False,
    useWalls: bool = False,
    sensorWalls=None,
    wallOperation: Optional[Callable] = None,
    save: bool = False,
    load: bool = False,
    plotRecoNames: str = 'grid'
):
    """
    Build per-time-bin grids by slicing a hits DataFrame, then calling a legacy
    grid maker (not shown here) that wraps the same traversal machinery.

    Parameters
    ----------
    df_hits : pandas.DataFrame
        Must contain at least the columns:
          * 'time_bin'
          * 'sensor_position'
          * 'reconstructedVector_direction'
          * 'sensor_wall'
          * 'initialPosition' (optional, only used for returning positions)
    nTimeBins : int
        How many distinct time bins to iterate over.
    shape, detectorDimensions
        Grid resolution and detector extents in mm.
    make_errors, useWalls, sensorWalls, wallOperation, save, load, plotRecoNames
        Passed through to the legacy maker.

    Returns
    -------
    grids, grids_ind, grids_pos, initialPositions : lists
        Per-time-bin outputs from the legacy path.
    """
    grids, grids_ind, grids_pos = [], [], []
    initialPositions = []

    for timeStep in range(nTimeBins):
        if isinstance(plotRecoNames, str):
            plotRecoName = 'data/{}_{}.npz'.format(plotRecoNames, timeStep)

        df_hits_time = df_hits[df_hits['time_bin'] == timeStep]
        sensorPositions = np.array(df_hits_time['sensor_position'              ].to_list()).reshape(-1,3)
        recoDirections  = np.array(df_hits_time['reconstructedVector_direction'].to_list()).reshape(-1,3)
        sensorWalls     =          df_hits_time['sensor_wall'                  ].to_numpy()
        hitWeights      = [1 for _ in range(len(df_hits_time))]

        # NOTE: This calls a separate, older "make_voxelGrid" not included here.
        # The modern entry point is `get_voxelGrid` above.
        grid, grid_ind, grid_pos = make_voxelGrid(
            df_hits=df_hits, 
            shape=shape, 
            detectorDimensions=detectorDimensions,
            sensorPositions=sensorPositions, 
            recoDirections=recoDirections, 
            hitWeights=hitWeights, 
            make_errors=make_errors,
            useWalls=useWalls, 
            sensorWalls=sensorWalls, 
            wallOperation=wallOperation,
            save=save, 
            load=load, 
            fileName=plotRecoNames
        )

        grids           .append(grid    )
        grids_ind       .append(grid_ind)
        grids_pos       .append(grid_pos)
        initialPositions.append(np.reshape(df_hits_time['initialPosition'].to_list(), (-1,3)))
    
    return grids, grids_ind, grids_pos, initialPositions


def make_voxelGrid_truth(
    trackPositions: np.ndarray, 
    shape: Tuple[int,int,int] = (10,10,10), 
    detectorDimensions: Iterable[float] = DETECTOR_SIZE_MM, 
    makeErrors: bool = False
) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
    """
    Rasterize a set of 3D points (e.g., ground-truth positions) into a grid.

    Each input position contributes +1 to the voxel it falls in. Out-of-bounds
    points are discarded. Returns both the dense grid and a sparse index/pos
    list for convenience.

    Parameters
    ----------
    trackPositions : (M,3) float
        Input positions in mm in detector coordinates (centered at 0).
    shape : (3,) int
        Grid resolution (nx,ny,nz).
    detectorDimensions : (3,) float
        Physical extents (Dx,Dy,Dz) in mm (bounds are [-D/2, +D/2]).
    makeErrors : bool
        Ignored (kept for legacy signature compatibility).

    Returns
    -------
    grid : (nx,ny,nz) float
        Voxelized counts.
    grid_ind : (K,3) int
        Indices of all voxels (not just non-zero) for convenience.
    grid_pos : (K,3) float
        Positions (lower corners) corresponding to `grid_ind`.
    """
    shape = np.array(shape, dtype=int).reshape(3)
    detectorDimensions = np.array(detectorDimensions, dtype=float).reshape(3)
    trackPositions = np.array(trackPositions, dtype=float).reshape(-1, 3) + detectorDimensions / 2.0

    scale = shape / detectorDimensions
    # Compute integer bins (floor gives [0..nx-1] for positions inside the box)
    positions = np.floor(trackPositions * scale).astype(int)

    # Keep only positions that fall inside the grid
    inside = (positions >= 0).all(axis=1) & (positions < shape).all(axis=1)
    positions = positions[inside]

    grid = np.zeros(shape, dtype=float)
    if positions.size:
        grid[positions[:, 0], positions[:, 1], positions[:, 2]] += 1.0

    grid_ind = np.argwhere(grid >= 0)
    grid_pos = grid_ind / scale - detectorDimensions[np.newaxis, :] / 2.0

    return grid, grid_ind, grid_pos
