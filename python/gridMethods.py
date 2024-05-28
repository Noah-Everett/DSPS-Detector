import numpy as np
import tqdm
from scipy.spatial.distance import cdist
from constants import *

def get_voxelGrid_hitVector(shape, starts, ends, weights):
    if weights is None:
        weights = np.ones(len(starts))
    elif len(weights) != len(starts):
        raise ValueError('weights must be None or a list of the same length as starts and ends')
    elif np.any(weights > 1) or np.any(weights <= 0):
        raise ValueError('weights must be None or a list of values between 0 and 1')

    grid = np.zeros(shape, dtype=float)

    def initializeVariables():
        x1, y1, z1 = starts[:, 0], starts[:, 1], starts[:, 2]
        x2, y2, z2 = ends  [:, 0], ends  [:, 1], ends  [:, 2]

        dx = np.abs(x2 - x1)
        dy = np.abs(y2 - y1)
        dz = np.abs(z2 - z1)

        sx = np.ones(len(starts), dtype=int); sx[x1 > x2] = -1
        sy = np.ones(len(starts), dtype=int); sy[y1 > y2] = -1
        sz = np.ones(len(starts), dtype=int); sz[z1 > z2] = -1

        w = weights

        return x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, w
    
    def updateVariables(indices, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w):
        x1, y1, z1 = x1[indices], y1[indices], z1[indices]
        x2, y2, z2 = x2[indices], y2[indices], z2[indices]
        d1, d2     = d1[indices], d2[indices]
        dx, dy, dz = dx[indices], dy[indices], dz[indices]
        sx, sy, sz = sx[indices], sy[indices], sz[indices]
        w          = w [indices]

        return x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w

    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, w = initializeVariables()
    X_ind = np.where((dx >= dy) & (dx >= dz))[0]
    Y_ind = np.where((dy >= dx) & (dy >= dz))[0]
    Y_ind = Y_ind[~np.isin(Y_ind, X_ind)]
    Z_ind = np.linspace(0, len(starts)-1, len(starts), dtype=int)
    Z_ind = Z_ind[~np.isin(Z_ind, np.concatenate((X_ind, Y_ind)))]

    # if X is the dominant axis
    d1 = dy - dx
    d2 = dz - dx
    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w = updateVariables(X_ind, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w)
    while np.where(x1 != x2)[0].size > 0:
        X_ind = np.where(x1 != x2)[0]
        x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w = updateVariables(X_ind, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w)

        X_cur = np.where((x1 >= 0) & (x1 < shape[0]) & 
                         (y1 >= 0) & (y1 < shape[1]) & 
                         (z1 >= 0) & (z1 < shape[2]))[0]
        np.add.at(grid, (x1[X_cur], y1[X_cur], z1[X_cur]), w[X_cur])

        x1 += sx

        X_cur = np.where(d1 >= 0)[0]
        y1[X_cur] += sy[X_cur]
        d1[X_cur] -= dx[X_cur]

        X_cur = np.where(d2 >= 0)[0]
        z1[X_cur] += sz[X_cur]
        d2[X_cur] -= dx[X_cur]

        d1 += dy
        d2 += dz

    # if Y is the dominant axis
    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, w = initializeVariables()
    d1 = dx - dy
    d2 = dz - dy
    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w = updateVariables(Y_ind, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w)
    while np.where(y1 != y2)[0].size > 0:
        Y_ind = np.where(y1 != y2)[0]
        x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w = updateVariables(Y_ind, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w)

        Y_cur = np.where((x1 >= 0) & (x1 < shape[0]) & 
                         (y1 >= 0) & (y1 < shape[1]) & 
                         (z1 >= 0) & (z1 < shape[2]))[0]
        np.add.at(grid, (x1[Y_cur], y1[Y_cur], z1[Y_cur]), w[Y_cur])

        y1 += sy

        Y_cur = np.where(d1 >= 0)[0]
        x1[Y_cur] += sx[Y_cur]
        d1[Y_cur] -= dy[Y_cur]

        Y_cur = np.where(d2 >= 0)[0]
        z1[Y_cur] += sz[Y_cur]
        d2[Y_cur] -= dy[Y_cur]

        d1 += dx
        d2 += dz

    # if Z is the dominant axis
    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, w = initializeVariables()
    d1 = dx - dz
    d2 = dy - dz
    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w = updateVariables(Z_ind, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w)
    while np.where(z1 != z2)[0].size > 0:
        Z_ind = np.where(z1 != z2)[0]
        x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w = updateVariables(Z_ind, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w)

        Z_cur = np.where((x1 >= 0) & (x1 < shape[0]) & 
                         (y1 >= 0) & (y1 < shape[1]) & 
                         (z1 >= 0) & (z1 < shape[2]))[0]
        np.add.at(grid, (x1[Z_cur], y1[Z_cur], z1[Z_cur]), w[Z_cur])

        z1 += sz

        Z_cur = np.where(d1 >= 0)[0]
        x1[Z_cur] += sx[Z_cur]
        d1[Z_cur] -= dz[Z_cur]

        Z_cur = np.where(d2 >= 0)[0]
        y1[Z_cur] += sy[Z_cur]
        d2[Z_cur] -= dz[Z_cur]

        d1 += dx
        d2 += dy

    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, w = initializeVariables()
    ind_cur = np.where((x2 >= 0) & (x2 < shape[0]) &
                       (y2 >= 0) & (y2 < shape[1]) &
                       (z2 >= 0) & (z2 < shape[2]))[0]
    np.add.at(grid, (x2[ind_cur], y2[ind_cur], z2[ind_cur]), weights[ind_cur])

    return grid

def get_voxelGrid_errors(grid):
    grid_copy = grid.copy()

    zeros = np.argwhere(grid_copy == 0)
    # print('zeros', zeros)
    # print('zeros.shape', zeros.shape)
    hit = np.argwhere(grid_copy > 0)
    # print('hit', hit)
    # print('hit.shape', hit.shape)
    # print('len(hit)', len(hit))
    if len(hit) == 0:
        return grid_copy
    distances = cdist(zeros, hit, metric='euclidean')
    # print('distances.shape', distances.shape)
    closestHit = np.argmin(distances, axis=1)
    # print('closestHit', closestHit)
    # print('closestHit.shape', closestHit.shape)
    distances = distances[np.arange(len(distances)), closestHit]
    # print('distances', distances)
    # print('distances.shape', distances.shape)
    closestHit_ind = hit[closestHit]
    # print('closestHit_ind', closestHit_ind)
    # print('closestHit_ind.shape', closestHit_ind.shape)
    hitValues = grid_copy[closestHit_ind[:, 0], closestHit_ind[:, 1], closestHit_ind[:, 2]]
    # print('hitValues', hitValues)
    # print('hitValues.shape', hitValues.shape)
    p = np.exp(-distances)
    # print('p', p)
    # print('p.shape', p.shape)
    # print('grid_copy.shape', grid_copy.shape)
    # print('p * hitValues', p * hitValues)
    # print('(p * hitValues).shape', (p * hitValues).shape)
    grid_copy[zeros[:, 0], zeros[:, 1], zeros[:, 2]] = p * hitValues

    return grid_copy

def get_voxelGrid(shape, detectorDimensions, 
                  sensorPositions, recoDirections, 
                  hitWeights=None, make_errors=False,
                  useWalls=False, sensorWalls=None, 
                  wallOperation=None, returnWalls=False):
    shape = np.array(shape).reshape(3)
    detectorDimensions = np.array(detectorDimensions).reshape(3)
    sensorPositions = np.array(sensorPositions).reshape(-1, 3)
    sensorPositions = sensorPositions + detectorDimensions / 2
    recoDirections = np.array(recoDirections).reshape(-1, 3)
    hitWeights = np.array(hitWeights).reshape(-1)
    if useWalls and sensorWalls is None:
        raise ValueError('sensorWalls must be a list of the same length as sensorPositions')

    grid = np.zeros(shape, dtype=float)

    scale = shape / detectorDimensions
    starts = (sensorPositions * scale).astype(int)
    ends = (starts + recoDirections * scale * np.max(shape)**2 / np.max(scale)).astype(int)

    wall_grids = {}

    if not useWalls and not returnWalls:
        grid = get_voxelGrid_hitVector(shape, starts, ends, hitWeights)
        if make_errors:
            grid = get_voxelGrid_errors(grid)#, max_distance=max_distance)
    if useWalls or returnWalls:
        for wall in [pm+dir for pm in ['+', '-'] for dir in ['x', 'y', 'z']]:
            wall_ind = np.where(sensorWalls == wall)[0]
            if len(wall_ind) == 0:
                wall_grids[wall] = np.zeros(shape, dtype=float)
                continue
            wall_starts = starts[wall_ind]
            wall_ends = ends[wall_ind]
            wall_hitWeights = hitWeights[wall_ind]
            wall_grid = get_voxelGrid_hitVector(shape, wall_starts, wall_ends, wall_hitWeights)
            if make_errors:
                wall_grid = get_voxelGrid_errors(wall_grid)
            wall_grids[wall] = wall_grid

    grid_ind = np.argwhere(grid >= 0)
    grid_pos = grid_ind / scale - detectorDimensions[np.newaxis, :] / 2

    if useWalls:
        grid = wallOperation(np.reshape(np.array(list(wall_grids.values())), (len(wall_grids), *shape)), list(wall_grids.keys()), returnWalls=returnWalls)
    elif returnWalls:
        grid = np.zeros(grid.shape + (6,))
        for nWall, wallName in enumerate(wall_grids.keys()):
            grid[:,:,:,nWall] += wall_grids[wallName]

    return grid, grid_ind, grid_pos

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