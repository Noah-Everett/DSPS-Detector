import numpy as np
import tqdm
from scipy.spatial.distance import cdist
from constants import *
from concurrent.futures import ProcessPoolExecutor
import numba

@numba.njit
def position_to_voxel_indices(x, y, z, x0_grid, y0_grid, z0_grid, dx, dy, dz, Nx, Ny, Nz):
    i = int((x - x0_grid) / dx)
    j = int((y - y0_grid) / dy)
    k = int((z - z0_grid) / dz)
    # Clamp to grid bounds
    i = max(0, min(Nx - 1, i))
    j = max(0, min(Ny - 1, j))
    k = max(0, min(Nz - 1, k))
    return i, j, k

@numba.njit
def traverse_voxel_line(x0, y0, z0, x1, y1, z1, x0_grid, y0_grid, z0_grid, dx_vox, dy_vox, dz_vox, Nx, Ny, Nz):
    dx = x1 - x0
    dy = y1 - y0
    dz = z1 - z0
    L = np.sqrt(dx * dx + dy * dy + dz * dz)
    if L == 0:
        return np.empty((0, 3), dtype=np.int32)
    vx = dx / L
    vy = dy / L
    vz = dz / L
    ix, iy, iz = position_to_voxel_indices(x0, y0, z0, x0_grid, y0_grid, z0_grid, dx_vox, dy_vox, dz_vox, Nx, Ny, Nz)
    ix1, iy1, iz1 = position_to_voxel_indices(x1, y1, z1, x0_grid, y0_grid, z0_grid, dx_vox, dy_vox, dz_vox, Nx, Ny, Nz)
    stepX = np.sign(vx)
    stepY = np.sign(vy)
    stepZ = np.sign(vz)
    if vx != 0:
        if stepX > 0:
            tVoxelBoundaryX = x0_grid + (ix + 1) * dx_vox
        else:
            tVoxelBoundaryX = x0_grid + ix * dx_vox
        tMaxX = (tVoxelBoundaryX - x0) / vx
        tDeltaX = dx_vox / abs(vx)
    else:
        tMaxX = np.inf
        tDeltaX = np.inf
    if vy != 0:
        if stepY > 0:
            tVoxelBoundaryY = y0_grid + (iy + 1) * dy_vox
        else:
            tVoxelBoundaryY = y0_grid + iy * dy_vox
        tMaxY = (tVoxelBoundaryY - y0) / vy
        tDeltaY = dy_vox / abs(vy)
    else:
        tMaxY = np.inf
        tDeltaY = np.inf
    if vz != 0:
        if stepZ > 0:
            tVoxelBoundaryZ = z0_grid + (iz + 1) * dz_vox
        else:
            tVoxelBoundaryZ = z0_grid + iz * dz_vox
        tMaxZ = (tVoxelBoundaryZ - z0) / vz
        tDeltaZ = dz_vox / abs(vz)
    else:
        tMaxZ = np.inf
        tDeltaZ = np.inf

    max_steps = Nx + Ny + Nz
    nsteps = 0
    voxel_indices = []
    while True:
        if 0 <= ix < Nx and 0 <= iy < Ny and 0 <= iz < Nz:
            voxel_indices.append((ix, iy, iz))
        else:
            break
        nsteps += 1
        if nsteps > max_steps:
            break
        if ix == ix1 and iy == iy1 and iz == iz1:
            break
        if tMaxX < tMaxY:
            if tMaxX < tMaxZ:
                ix += int(stepX)
                tMaxX += tDeltaX
            else:
                iz += int(stepZ)
                tMaxZ += tDeltaZ
        else:
            if tMaxY < tMaxZ:
                iy += int(stepY)
                tMaxY += tDeltaY
            else:
                iz += int(stepZ)
                tMaxZ += tDeltaZ
    return np.array(voxel_indices, dtype=np.int32)

def get_voxelGrid_hitVector(centers, starts, ends, weights):
    """
    centers: numpy array of shape (Nx, Ny, Nz, 3)
    starts: numpy array of shape (N, 3)
    ends: numpy array of shape (N, 3)
    weights: numpy array of shape (N,)
    """
    Nx, Ny, Nz, _ = centers.shape

    # Corrected voxel size calculation for uniform grid
    if Nx > 1:
        dx_vox = centers[1, 0, 0, 0] - centers[0, 0, 0, 0]
    else:
        dx_vox = 1.0
    if Ny > 1:
        dy_vox = centers[0, 1, 0, 1] - centers[0, 0, 0, 1]
    else:
        dy_vox = 1.0
    if Nz > 1:
        dz_vox = centers[0, 0, 1, 2] - centers[0, 0, 0, 2]
    else:
        dz_vox = 1.0

    x0_grid = centers[0, 0, 0, 0] - dx_vox / 2
    y0_grid = centers[0, 0, 0, 1] - dy_vox / 2
    z0_grid = centers[0, 0, 0, 2] - dz_vox / 2

    grid = np.zeros((Nx, Ny, Nz))

    for i in numba.prange(len(starts)):
        x0, y0, z0 = starts[i]
        x1, y1, z1 = ends[i]
        w = weights[i]
        voxel_indices = traverse_voxel_line(
            x0, y0, z0, x1, y1, z1,
            x0_grid, y0_grid, z0_grid,
            dx_vox, dy_vox, dz_vox,
            Nx, Ny, Nz)
        for idx in voxel_indices:
            ix, iy, iz = idx
            grid[ix, iy, iz] += w

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


def process_wall(wall, indices, starts, ends, hitWeights, centers, make_errors):
    if indices.size == 0:
        return wall, np.zeros(centers.shape[:3], dtype=float)
    wall_starts = starts[indices]
    wall_ends = ends[indices]
    wall_weights = hitWeights[indices]
    wall_grid = get_voxelGrid_hitVector(centers, wall_starts, wall_ends, wall_weights)
    if make_errors:
        wall_grid = get_voxelGrid_errors(wall_grid)
    return wall, wall_grid

def process_wall_wrapper(args):
    return process_wall(*args)

def get_voxelGrid(shape, detectorDimensions, 
                  sensorPositions, recoDirections, 
                  hitWeights=None, make_errors=False,
                  useWalls=False, sensorWalls=None, 
                  wallOperation=None, returnWalls=False):
    """
    Computes the voxel grid based on sensor positions and reconstructed directions.

    Parameters:
    - shape: Tuple or list of 3 integers (Nx, Ny, Nz), the shape of the grid.
    - detectorDimensions: Tuple or list of 3 floats, physical dimensions of the detector.
    - sensorPositions: Array of shape (N, 3), positions of the sensors.
    - recoDirections: Array of shape (N, 3), reconstructed directions.
    - hitWeights: Optional array of shape (N,), weights for each hit.
    - make_errors: Boolean flag to apply error processing.
    - useWalls: Boolean flag indicating whether to process walls separately.
    - sensorWalls: Optional array of shape (N,), wall identifiers for each sensor.
    - wallOperation: Function to combine wall grids.
    - returnWalls: Boolean flag indicating whether to return wall grids.

    Returns:
    - grid: The computed voxel grid.
    - grid_indices: Indices of the grid where values are non-zero.
    - grid_positions: Physical positions corresponding to grid indices.
    """
    shape = np.array(shape, dtype=int).reshape(3)
    detectorDimensions = np.array(detectorDimensions, dtype=float).reshape(3)
    Nx, Ny, Nz = shape
    dx_vox = detectorDimensions[0] / Nx
    dy_vox = detectorDimensions[1] / Ny
    dz_vox = detectorDimensions[2] / Nz

    # Compute grid centers
    x0_grid = -detectorDimensions[0] / 2.0
    y0_grid = -detectorDimensions[1] / 2.0
    z0_grid = -detectorDimensions[2] / 2.0

    x_centers = x0_grid + (np.arange(Nx) + 0.5) * dx_vox
    y_centers = y0_grid + (np.arange(Ny) + 0.5) * dy_vox
    z_centers = z0_grid + (np.arange(Nz) + 0.5) * dz_vox

    centers = np.zeros((Nx, Ny, Nz, 3), dtype=np.float64)
    centers[:, :, :, 0] = x_centers[:, None, None]
    centers[:, :, :, 1] = y_centers[None, :, None]
    centers[:, :, :, 2] = z_centers[None, None, :]

    sensorPositions = np.array(sensorPositions, dtype=float).reshape(-1, 3)
    recoDirections = np.array(recoDirections, dtype=float).reshape(-1, 3)

    if hitWeights is None:
        hitWeights = np.ones(len(sensorPositions), dtype=float)
    else:
        hitWeights = np.array(hitWeights, dtype=float).reshape(-1)
        
    if useWalls and sensorWalls is None:
        raise ValueError('sensorWalls must be provided when useWalls is True')

    # Starts are sensor positions
    starts = sensorPositions

    # Compute ends by extending the reconstructed directions
    max_distance = np.linalg.norm(detectorDimensions) * 2.0  # A large distance to ensure traversal through the grid
    ends = sensorPositions + recoDirections * max_distance

    grid = np.zeros(shape, dtype=float)
    wall_grids = {}

    if not useWalls and not returnWalls:
        grid = get_voxelGrid_hitVector(centers, starts, ends, hitWeights)
        if make_errors:
            grid = get_voxelGrid_errors(grid)
    else:
        walls = ['+x', '-x', '+y', '-y', '+z', '-z']
        wall_indices = {wall: np.flatnonzero(sensorWalls == wall) for wall in walls}

        # Prepare arguments for each wall
        args_list = [
            (
                wall,
                wall_indices[wall],
                starts,
                ends,
                hitWeights,
                centers,
                make_errors
            ) for wall in walls
        ]

        # Use ProcessPoolExecutor for parallel processing
        with ProcessPoolExecutor() as executor:
            results = executor.map(process_wall_wrapper, args_list)
        wall_grids = dict(results)

    # Compute grid indices and positions
    if not useWalls and not returnWalls:
        grid_indices = np.argwhere(grid > 0)
    else:
        grid_indices = None  # Adjust as needed

    scale = shape / detectorDimensions
    grid_positions = grid_indices / scale - detectorDimensions / 2.0 if grid_indices is not None else None

    if useWalls:
        wall_arrays = np.stack([wall_grids[wall] for wall in walls], axis=0)
        grid = wallOperation(wall_arrays, walls, returnWalls=returnWalls)
    elif returnWalls:
        grid = np.stack([wall_grids[wall] for wall in walls], axis=-1)

    return grid, grid_indices, grid_positions

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

def make_voxelGrid(df_hits=None, 
                   shape=(10,10,10), detectorDimensions=DETECTOR_SIZE_MM,
                   sensorPositions=None, recoDirections=None, 
                   hitWeights=None, make_errors=False,
                   useWalls=False, sensorWalls=None, 
                   wallOperation=None, returnWalls=False,
                   save=False, load=False, fileName='data/grid.npz'):
    if type(sensorPositions) == None and sensorPositions == None:
        if df_hits == None:
            raise ValueError('Either sensorPosition or df_hits must be given, i.e. not None')
        sensorPositions = np.array(df_hits['sensor_position'].to_list()).reshape(-1,3)
    if type(recoDirections) == None and recoDirections == None:
        if df_hits == None:
            raise ValueError('Either recoDirections or df_hits must be given, i.e. not None')
        recoDirections = np.array(df_hits['reconstructedVector_direction'].to_list()).reshape(-1,3)
    if type(hitWeights) == None and hitWeights == None:
        if df_hits == None:
            raise ValueError('Either hitWeights or df_hits must be given, i.e. not None')
        hitWeights = [1 for i in range(len(df_hits))]

    if not load:
        grid, grid_ind, grid_pos = get_voxelGrid(
            shape=shape,
            detectorDimensions=detectorDimensions,
            sensorPositions=sensorPositions, 
            recoDirections=recoDirections, 
            hitWeights=hitWeights,
            make_errors=make_errors,
            useWalls=useWalls,
            sensorWalls=sensorWalls,
            wallOperation=wallOperation,
            returnWalls=returnWalls
        )
        if save:
            np.savez(fileName, grid=grid, grid_ind=grid_ind, grid_pos=grid_pos)
    else:
        data = np.load(fileName)
        grid     = data['grid'    ]
        # grid = get_voxelGrid_errors(grid)
        grid_ind = data['grid_ind']
        grid_pos = data['grid_pos']

    return grid, grid_ind, grid_pos

def make_voxelGrids_withTimeSteps(df_hits, nTimeBins, 
                                  shape=(10,10,10), detectorDimensions=DETECTOR_SIZE_MM,
                                  make_errors=False,
                                  useWalls=False, sensorWalls=None, wallOperation=None,
                                  save=False, load=False, plotRecoNames='grid'):
    grids, grids_ind, grids_pos = [], [], []
    initialPositions = []

    for timeStep in tqdm.tqdm(range(nTimeBins)):
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