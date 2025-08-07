import numpy as np
from scipy.spatial.distance import cdist
from constants import *
import fast_voxel_traversal as fvt
from collections import defaultdict

def get_voxelGrid_hitVector(grid_minBound, grid_maxBound, grid_shape, 
                            vector_starts, vector_ends  , vector_weights=None):
    """
    Compute the voxel grid from the hit vectors.

    Parameters:
    - grid_minBound: Tuple or list of 3 floats, minimum bounds of the grid.
    - grid_maxBound: Tuple or list of 3 floats, maximum bounds of the grid.
    - grid_shape: Tuple or list of 3 integers, shape of the grid.
    - vector_starts: Array of shape (N, 3), starting points of the vectors.
    - vector_ends: Array of shape (N, 3), ending points of the vectors.
    - vector_weights: Array of shape (N,), weights for each vector.
    """

    if vector_weights is None:
        vector_weights = np.ones(len(vector_starts), dtype=float)

    assert len(vector_starts) == len(vector_ends) == len(vector_weights)
    assert len(grid_minBound) == len(grid_maxBound) == len(grid_shape) == 3

    # Calculate voxel size from bounds and shape
    voxel_size = (np.array(grid_maxBound) - np.array(grid_minBound)) / np.array(grid_shape)
    
    # Create FVT grid
    FVTgrid = fvt.Grid(grid_shape=grid_shape, voxel_size=voxel_size, grid_origin=grid_minBound)
    grid = np.zeros(grid_shape, dtype=float)

    for i in range(len(vector_starts)):
        # Calculate direction vector from start to end
        direction = vector_ends[i] - vector_starts[i]
        
        # Traverse the voxels and collect indices
        traversed_indices = []
        for ix, iy, iz, t_enter, t_exit in FVTgrid.traverse(vector_starts[i], direction):
            traversed_indices.append([ix, iy, iz])
        
        if traversed_indices:
            traversed_indices = np.array(traversed_indices)
            grid[traversed_indices[:, 0], traversed_indices[:, 1], traversed_indices[:, 2]] += vector_weights[i]

    return grid

def get_voxelGrid_errors(grid):
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

def get_voxelGrid_ind(size):
    grid_ind = np.indices(size).reshape(3, -1).T
    return grid_ind

def get_voxelGrid_pos(size, detectorDimensions):
    if type(size) != np.ndarray:
        size = np.array(size).reshape(3)
    if type(detectorDimensions) != np.ndarray:
        detectorDimensions = np.array(detectorDimensions).reshape(3)

    scale = size / detectorDimensions
    grid_ind = get_voxelGrid_ind(size)
    grid_pos = grid_ind / scale - detectorDimensions[np.newaxis, :] / 2

    return grid_pos

def get_voxelGrid_centers(size, detectorDimensions):
    """
    Computes the centers of the voxels in the grid.
    Note: This has not been tested. It was written entirely by Copilot.
    """

    if type(size) != np.ndarray:
        size = np.array(size).reshape(3)
    if type(detectorDimensions) != np.ndarray:
        detectorDimensions = np.array(detectorDimensions).reshape(3)

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

def get_vectors_end(vector_starts: np.ndarray,
                    vector_directions: np.ndarray,
                    grid_minBound: np.ndarray,
                    grid_maxBound: np.ndarray,
                    epsilon: float = 1e-6) -> np.ndarray:
    """
    Computes the end points of vectors that traverse the grid.

    Parameters:
    - vector_starts: Array of shape (N, 3), starting points of the vectors.
    - vector_directions: Array of shape (N, 3), directions of the vectors.
    - grid_minBound: Tuple or list of 3 floats, minimum bounds of the grid.
    - grid_maxBound: Tuple or list of 3 floats, maximum bounds of the grid.

    Returns:
    - Array of shape (N, 3) containing the end points of the vectors within the grid bounds.
    """

    assert vector_starts.shape == vector_directions.shape
    assert grid_minBound.shape == grid_maxBound.shape == (3,)

    t_min = np.where(vector_directions != 0, (grid_minBound - vector_starts) / vector_directions, -np.inf)
    t_max = np.where(vector_directions != 0, (grid_maxBound - vector_starts) / vector_directions,  np.inf)

    # Calculate the minimum and maximum t-values for each axis
    t_enter = np.minimum(t_min, t_max)
    t_exit = np.maximum(t_min, t_max)

    # Get the largest t_enter and smallest t_exit for each vector
    t_start = np.max(t_enter, axis=1)
    t_end = np.min(t_exit, axis=1)

    # Filter for positive t_start and t_end
    t_positive = t_end > t_start
    valid_vectors = np.where(t_positive, t_start, np.nan)

    # Calculate the end points of valid vectors
    vector_ends = vector_starts + np.expand_dims(valid_vectors, axis=1) * vector_directions

    # Make sure the end points are within the grid bounds
    vector_ends = np.where(vector_ends >= grid_maxBound, grid_maxBound - epsilon, vector_ends)
    assert (grid_minBound <= vector_ends).all()
    assert (vector_ends < grid_maxBound).all()

    return vector_ends

def get_indexFromPosition(position: np.ndarray,
                          grid_minBound: np.ndarray,
                          grid_maxBound: np.ndarray,
                          grid_shape: np.ndarray) -> np.ndarray:
    """
    Get the indices of multiple positions within the grid.

    Parameters:
    - position: Array of shape (N, 3), positions to get the indices of.
    - grid_minBound: Array of shape (3,), minimum bounds of the grid.
    - grid_maxBound: Array of shape (3,), maximum bounds of the grid.
    - grid_shape: Array of shape (3,), shape of the grid.

    Returns:
    - Array of shape (N, 3) containing the indices of the positions within the grid.
    """
    
    assert position.shape[1] == 3, "Position array should be of shape (N, 3)"
    assert grid_minBound.shape == grid_maxBound.shape == grid_shape.shape == (3,)
    assert (grid_minBound <= position).all() and (position < grid_maxBound).all()

    scale = (grid_maxBound - grid_minBound) / grid_shape
    indices = ((position - grid_minBound) / scale).astype(int)

    assert (0 <= indices).all()
    assert (indices < grid_shape).all()

    return indices

def get_positionFromIndex(index: np.ndarray,
                          grid_minBound: np.ndarray,
                          grid_maxBound: np.ndarray,
                          grid_shape: np.ndarray) -> np.ndarray:
    """
    Get the positions of multiple indices within the grid.

    Parameters:
    - index: Array of shape (N, 3), indices to get the positions of.
    - grid_minBound: Array of shape (3,), minimum bounds of the grid.
    - grid_maxBound: Array of shape (3,), maximum bounds of the grid.
    - grid_shape: Array of shape (3,), shape of the grid.

    Returns:
    - Array of shape (N, 3) containing the positions of the indices within the grid.
    """
    
    assert index.shape[1] == 3, "Index array should be of shape (N, 3)"
    assert grid_minBound.shape == grid_maxBound.shape == grid_shape.shape == (3,)
    assert (0 <= index).all() and (index < grid_shape).all()

    scale = (grid_maxBound - grid_minBound) / grid_shape
    positions = grid_minBound + index * scale

    return positions

def wallStringToInt(walls_str):
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

def get_combinedVectors(vector_starts_ind: np.ndarray,
                        vector_ends_ind: np.ndarray,
                        vector_weights: np.ndarray = None) -> np.ndarray:
    """
    Combines vectors with the same start and end points.

    Parameters:
    - vector_starts_ind: Array of shape (N, 3), index of the starting points of the vectors.
    - vector_ends_ind: Array of shape (N, 3), index of the end points of the vectors.
    - vector_weights: Array of shape (N,), weights for each vector.

    Returns:
    - Array of shape (M, 3), index of the starting points of the combined vectors.
    - Array of shape (M, 3), index of the end points of the combined vectors.
    - Array of shape (M,), weights for each combined vector (sum of the weights of the combined vectors).
    - Array of shape (N,), index of the first vector in each combined vector.
    """

    if vector_weights is None:
        vector_weights = np.ones(len(vector_starts_ind), dtype=float)

    assert vector_starts_ind.shape == vector_ends_ind.shape == (len(vector_weights), 3)
    assert vector_weights.shape == (len(vector_weights),)

    # Dictionary to accumulate weights for each unique (start, end) pair and store their original indices
    vector_dict = defaultdict(lambda: {"weight": 0.0, "index": -1})

    # Iterate over the vectors and accumulate weights
    for i, (start, end, weight) in enumerate(zip(map(tuple, vector_starts_ind), map(tuple, vector_ends_ind), vector_weights)):
        vector_dict[(start, end)]["weight"] += weight
        if vector_dict[(start, end)]["index"] == -1:
            vector_dict[(start, end)]["index"] = i

    # Get the combined vectors
    combined_vectors       = np.array(list(vector_dict.keys()))
    combined_vectors_start = np.array([i[0] for i in combined_vectors])
    combined_vectors_end   = np.array([i[1] for i in combined_vectors])
    combined_weights       = np.array([i["weight"] for i in vector_dict.values()])
    combined_indices       = np.array([i["index"] for i in vector_dict.values()])

    return combined_vectors_start, combined_vectors_end, combined_weights, combined_indices

def get_voxelGrid(grid_shape: np.ndarray, 
                  grid_dimensions: np.ndarray, 
                  vector_starts: np.ndarray,
                  vector_directions: np.ndarray,
                  vector_weights: np.ndarray = None,
                  vector_start_walls: np.ndarray = None,
                  walls: bool = False,
                  walls_combine: bool = False,
                  walls_combine_method: callable = None,
                  vector_combine: bool = False) -> np.ndarray:
    """
    Computes the voxel grid based on sensor positions and reconstructed directions.

    Parameters:
    - grid_shape: Array of shape (3,), shape of the voxel grid.
    - grid_dimensions: Array of shape (3,), dimensions of the voxel grid.
    - vector_starts: Array of shape (N, 3), starting points of the vectors.
    - vector_directions: Array of shape (N, 3), directions of the vectors.
    - vector_weights: Array of shape (N,), weights for each vector.
    - vector_start_walls: Array of strings of shape (N,), wall names for each vector (e.g. '+x', '-y', etc.).
    - walls: Boolean, whether to use walls.
    - walls_combine: Boolean, whether to combine walls.
    - walls_combine_method: String, method to combine walls.
    - vector_combine: Boolean, whether to combine all vectors with the same end points.

    Returns:
    - grid: Array of shape grid_shape, voxel grid.
    """

    if vector_weights is None:
        vector_weights = np.ones(len(vector_starts), dtype=float)

    assert len(grid_shape) == 3 and all(isinstance(i, int) or isinstance(i, np.int64) for i in grid_shape)
    assert len(grid_dimensions) == 3 and all(isinstance(i, float) for i in grid_dimensions)
    assert len(vector_starts) == len(vector_directions) == len(vector_weights) and vector_starts.shape[1] == 3 and len(vector_starts.shape) == 2
    assert not walls or vector_start_walls is not None
    assert not walls or not walls_combine or walls_combine_method is not None

    walls_intOptions = wallStringToInt(np.array(['+x', '-x', '+y', '-y', '+z', '-z']))
    assert np.min(vector_start_walls) >= 0

    grid_bounds_min = grid_dimensions / -2
    grid_bounds_max = grid_dimensions / 2

    vector_ends = get_vectors_end(vector_starts, vector_directions, grid_bounds_min, grid_bounds_max)

    if vector_combine:
        vector_starts_ind = get_indexFromPosition(vector_starts, grid_bounds_min, grid_bounds_max, grid_shape)
        vector_ends_ind   = get_indexFromPosition(vector_ends  , grid_bounds_min, grid_bounds_max, grid_shape)
        lenInit = len(vector_starts_ind)

        vector_starts_ind, vector_ends_ind, vector_weights, inds = get_combinedVectors(vector_starts_ind, vector_ends_ind, vector_weights)
        lenFinal = len(vector_starts_ind)
        # print(f"Combined vectors: {lenInit} -> {lenFinal} ({100 * lenFinal / lenInit:.2f}%)")

        vector_starts = get_positionFromIndex(vector_starts_ind, grid_bounds_min, grid_bounds_max, grid_shape)
        vector_ends   = get_positionFromIndex(vector_ends_ind  , grid_bounds_min, grid_bounds_max, grid_shape)

        vector_start_walls = vector_start_walls[inds]

    grid = None
    grid_walls = {}

    if not walls:
        grid = get_voxelGrid_hitVector(grid_bounds_min, grid_bounds_max, grid_shape, vector_starts, vector_ends, vector_weights)
        assert grid.shape == tuple(grid_shape)
    else:
        wall_indices = {wall: np.flatnonzero(vector_start_walls == wall) for wall in walls_intOptions}

        for wall in walls_intOptions:
            if len(wall_indices[wall]) == 0:
                continue

            wall_starts = vector_starts[wall_indices[wall]]
            wall_ends = vector_ends[wall_indices[wall]]
            wall_weights = vector_weights[wall_indices[wall]]

            grid_walls[wall] = get_voxelGrid_hitVector(grid_bounds_min, grid_bounds_max, grid_shape, wall_starts, wall_ends, wall_weights)

        if walls_combine:
            grid = walls_combine_method(list(grid_walls.values()))
        else:
            grid = np.zeros([*grid_shape, 6], dtype=float)
            for wall in walls_intOptions:
                grid[:, :, :, wall] = grid_walls[wall]

    return grid

def expNWalls(wallGrids, walls, nWalls=6, returnWalls=False):
    grid_sum = np.sum(wallGrids, axis=0)
    grid_walls = np.full(grid_sum.shape + (nWalls,), False, dtype=bool)

    for i, wall in enumerate(walls):
        grid_walls[wallGrids[i] > 0, i] = True

    grid_nWalls = np.sum(grid_walls, axis=3)
    for i in np.where(grid_nWalls == 2)[0]:
        i = np.unravel_index(i, grid_nWalls.shape)
        ind = grid_walls[i]
        if ind[0] and ind[1]:
            grid_nWalls[i] = 1
        elif ind[2] and ind[3]:
            grid_nWalls[i] = 1
        elif ind[4] and ind[5]:
            grid_nWalls[i] = 1
        
    # nonZeroInd = np.where(grid_sum > 0)
    # grid_final = grid_sum
    # grid_final[nonZeroInd] = np.power(grid_sum[nonZeroInd], grid_nWalls[nonZeroInd])

    grid_final = np.zeros(grid_sum.shape, dtype=float)
    # grid_final[grid_nWalls > 1] = 1
    grid_final[grid_nWalls > 1] = grid_sum[grid_nWalls > 1]

    if not returnWalls:
        return grid_final
    
    grid_finalWithWalls = np.zeros(grid_sum.shape + (nWalls,))
    for i in range(nWalls):
        grid_finalWithWalls[:,:,:,i] = np.where(grid_final <= 0, 0, wallGrids[i])

    return grid_finalWithWalls

def make_voxelGrids_withTimeSteps(df_hits, nTimeBins, 
                                  shape=(10,10,10), detectorDimensions=DETECTOR_SIZE_MM,
                                  make_errors=False,
                                  useWalls=False, sensorWalls=None, wallOperation=None,
                                  save=False, load=False, plotRecoNames='grid'):
    grids, grids_ind, grids_pos = [], [], []
    initialPositions = []

    for timeStep in range(nTimeBins):
        if type(plotRecoNames) == str:
            plotRecoName = 'data/{}_{}.npz'.format(plotRecoNames, timeStep)

        df_hits_time = df_hits[df_hits['time_bin']==timeStep]
        sensorPositions = np.array(df_hits_time['sensor_position'              ].to_list()).reshape(-1,3)
        recoDirections  = np.array(df_hits_time['reconstructedVector_direction'].to_list()).reshape(-1,3)
        sensorWalls     =          df_hits_time['sensor_wall'                  ].to_numpy()
        hitWeights      = [1 for i in range(len(df_hits_time))]

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

def make_voxelGrid_truth(trackPositions, 
                         shape=(10,10,10), 
                         detectorDimensions=DETECTOR_SIZE_MM, 
                         makeErrors=False):
    shape = np.array(shape).reshape(3)
    detectorDimensions = np.array(detectorDimensions).reshape(3)
    trackPositions = np.array(trackPositions).reshape(-1, 3) + detectorDimensions / 2

    scale = shape / detectorDimensions
    # positions = np.round(trackPositions * scale).astype(int)
    # positions -= (positions < 0).astype(float)
    positions = np.floor(trackPositions * scale).astype(int)

    positions = positions[(positions >= 0).all(axis=1) & (positions < shape).all(axis=1)]

    grid = np.zeros(shape, dtype=float)
    grid[positions[:, 0], positions[:, 1], positions[:, 2]] += 1

    grid_ind = np.argwhere(grid >= 0)
    grid_pos = grid_ind / scale - detectorDimensions[np.newaxis, :] / 2

    return grid, grid_ind, grid_pos