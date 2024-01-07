import numpy as np
import tqdm
from scipy.spatial.distance import cdist

def get_voxelGrid_hitVector(shape, start, end, weight, verbose=False):
    if weight == None:
        weight = 1
    elif weight > 1 or weight <= 0:
        raise ValueError('weight must be None or a value between 0 and 1')

    grid = np.zeros(shape, dtype=float)
    x1, y1, z1 = start
    x2, y2, z2 = end

    dx = abs(x2 - x1)
    dy = abs(y2 - y1)
    dz = abs(z2 - z1)

    sx = 1 if x1 < x2 else -1
    sy = 1 if y1 < y2 else -1
    sz = 1 if z1 < z2 else -1

    possiblePositions = np.argwhere(grid == 0).tolist()
    possiblePositions = [tuple(position) for position in possiblePositions]

    if dx >= dy and dx >= dz:
        # X is the dominant axis
        d1 = dy - dx
        d2 = dz - dx
        while x1 != x2:
            if (x1, y1, z1) in possiblePositions:
                grid[x1, y1, z1] += weight
            elif verbose:
                print('Warning: Non-endpoint out of bounds')
            x1 += sx
            if d1 >= 0:
                y1 += sy
                d1 -= dx
            if d2 >= 0:
                z1 += sz
                d2 -= dx
            d1 += dy
            d2 += dz

    elif dy >= dx and dy >= dz:
        # Y is the dominant axis
        d1 = dx - dy
        d2 = dz - dy
        while y1 != y2:
            if (x1, y1, z1) in possiblePositions:
                grid[x1, y1, z1] += weight
            elif verbose:
                print('Warning: Non-endpoint out of bounds')
            y1 += sy
            if d1 >= 0:
                x1 += sx
                d1 -= dy
            if d2 >= 0:
                z1 += sz
                d2 -= dy
            d1 += dx
            d2 += dz

    else:
        # Z is the dominant axis
        d1 = dx - dz
        d2 = dy - dz
        while z1 != z2:
            if (x1, y1, z1) in possiblePositions:
                grid[x1, y1, z1] += weight
            elif verbose:
                print('Warning: Non-endpoint out of bounds')
            z1 += sz
            if d1 >= 0:
                x1 += sx
                d1 -= dz
            if d2 >= 0:
                y1 += sy
                d2 -= dz
            d1 += dx
            d2 += dy

    # Add the last point
    if (x2, y2, z2) in possiblePositions:
        grid[x2, y2, z2] += weight
    elif verbose:
        print('Warning: Endpoint out of bounds')

    return grid

def get_voxelGrid_hitVector_batch(shape, starts, ends, weights, verbose=False):
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
    print('X_ind', X_ind)
    print('Y_ind', Y_ind)
    print('Z_ind', Z_ind)

    # if X is the dominant axis
    d1 = dy - dx
    d2 = dz - dx
    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w = updateVariables(X_ind, x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, d1, d2, w)
    print('starting X')
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
    print('starting Y')
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
    print('starting Z')
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

    print('starting last')
    x1, y1, z1, x2, y2, z2, dx, dy, dz, sx, sy, sz, w = initializeVariables()
    ind_cur = np.where((x2 >= 0) & (x2 < shape[0]) &
                       (y2 >= 0) & (y2 < shape[1]) &
                       (z2 >= 0) & (z2 < shape[2]))[0]
    np.add.at(grid, (x2[ind_cur], y2[ind_cur], z2[ind_cur]), weights[ind_cur])

    # for start, end, weight in tqdm.tqdm(zip(starts, ends, weights), total=len(starts)):
    #     x1, y1, z1 = start
    #     x2, y2, z2 = end

    #     dx = abs(x2 - x1)
    #     dy = abs(y2 - y1)
    #     dz = abs(z2 - z1)

    #     sx = 1 if x1 < x2 else -1
    #     sy = 1 if y1 < y2 else -1
    #     sz = 1 if z1 < z2 else -1

    #     possible_positions = np.argwhere(grid == 0).tolist()
    #     possible_positions = [tuple(position) for position in possible_positions]

    #     if dx >= dy and dx >= dz:
    #         # X is the dominant axis
    #         d1 = dy - dx
    #         d2 = dz - dx
    #         while x1 != x2:
    #             if (x1, y1, z1) in possible_positions:
    #                 grid[x1, y1, z1] += weight
    #             elif verbose:
    #                 print('Warning: Non-endpoint out of bounds')
    #             x1 += sx
    #             if d1 >= 0:
    #                 y1 += sy
    #                 d1 -= dx
    #             if d2 >= 0:
    #                 z1 += sz
    #                 d2 -= dx
    #             d1 += dy
    #             d2 += dz

    #     elif dy >= dx and dy >= dz:
    #         # Y is the dominant axis
    #         d1 = dx - dy
    #         d2 = dz - dy
    #         while y1 != y2:
    #             if (x1, y1, z1) in possible_positions:
    #                 grid[x1, y1, z1] += weight
    #             elif verbose:
    #                 print('Warning: Non-endpoint out of bounds')
    #             y1 += sy
    #             if d1 >= 0:
    #                 x1 += sx
    #                 d1 -= dy
    #             if d2 >= 0:
    #                 z1 += sz
    #                 d2 -= dy
    #             d1 += dx
    #             d2 += dz

    #     else:
    #         # Z is the dominant axis
    #         d1 = dx - dz
    #         d2 = dy - dz
    #         while z1 != z2:
    #             if (x1, y1, z1) in possible_positions:
    #                 grid[x1, y1, z1] += weight
    #             elif verbose:
    #                 print('Warning: Non-endpoint out of bounds')
    #             z1 += sz
    #             if d1 >= 0:
    #                 x1 += sx
    #                 d1 -= dz
    #             if d2 >= 0:
    #                 y1 += sy
    #                 d2 -= dz
    #             d1 += dx
    #             d2 += dy

    #     # Add the last point
    #     if (x2, y2, z2) in possible_positions:
    #         grid[x2, y2, z2] += weight
    #     elif verbose:
    #         print('Warning: Endpoint out of bounds')

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

# def get_voxelGrid_errors(grid, max_distance=10):
#     grid_copy = grid.copy()

#     points = np.argwhere(grid_copy >= 0).tolist()
#     points = [tuple(point) for point in points]

#     points_hit = np.argwhere(grid_copy > 0).tolist()
#     points_hit = [tuple(point) for point in points_hit]
    
#     for i in tqdm.tqdm(range(max_distance+1)):
#         p = np.exp(-i)
        
#         for point in points_hit:
#             x, y, z = point
            
#             # Generate all possible new points within the distance
#             new_points = [(x + dx, y + dy, z + dz) for dx in range(-i, i + 1)
#                           for dy in range(-i, i + 1) 
#                           for dz in range(-i, i + 1)]
            
#             # Filter new points that are in the list of existing points
#             valid_new_points = filter(lambda np: np in points, new_points)
#             valid_new_points = [(x, y, z) for x, y, z in valid_new_points if (x, y, z) != point]

#             # Update grid_copy for valid new points
#             for new_point in valid_new_points:
#                 if grid_copy[new_point] < p * grid_copy[point]:
#                     grid_copy[new_point] = p * grid_copy[point]

#     return grid_copy

def get_voxelGrid(shape, detectorDimensions, 
                  sensorPositions, recoDirections, 
                  hitWeights=None, max_distance=10, 
                  make_errors=False, verbose=False):
    shape = np.array(shape).reshape(3)
    detectorDimensions = np.array(detectorDimensions).reshape(3)
    sensorPositions = np.array(sensorPositions).reshape(-1, 3)
    sensorPositions = sensorPositions + detectorDimensions / 2
    recoDirections = np.array(recoDirections).reshape(-1, 3)
    hitWeights = np.array(hitWeights).reshape(-1)

    grid = np.zeros(shape, dtype=float)

    scale = shape / detectorDimensions
    starts = (sensorPositions * scale).astype(int)
    ends = (starts + recoDirections * scale * np.max(shape)**2 / np.max(scale)).astype(int)

    # for start, end, weight in tqdm.tqdm(zip(starts, ends, hitWeights), total=len(starts)):
    #     grid += get_voxelGrid_hitVector(shape, start, end, weight, verbose=verbose)
    grid = get_voxelGrid_hitVector_batch(shape, starts, ends, hitWeights, verbose=verbose)

    if make_errors:
        grid = get_voxelGrid_errors(grid)#, max_distance=max_distance)

    grid_ind = np.argwhere(grid >= 0)
    grid_pos = grid_ind / scale - detectorDimensions[np.newaxis, :] / 2

    return grid, grid_ind, grid_pos