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

def get_voxelGrid_errors(grid):
    grid_copy = grid.copy()

    zeros = np.argwhere(grid_copy == 0)
    print('zeros', zeros)
    print('zeros.shape', zeros.shape)
    hit = np.argwhere(grid_copy > 0)
    print('hit', hit)
    print('hit.shape', hit.shape)
    distances = cdist(zeros, hit, metric='chebyshev')
    print('distances.shape', distances.shape)
    closestHit = np.argmin(distances, axis=1)
    print('closestHit', closestHit)
    print('closestHit.shape', closestHit.shape)
    distances = distances[np.arange(len(distances)), closestHit]
    print('distances', distances)
    print('distances.shape', distances.shape)
    closestHit_ind = hit[closestHit]
    print('closestHit_ind', closestHit_ind)
    print('closestHit_ind.shape', closestHit_ind.shape)
    hitValues = grid_copy[closestHit_ind[:, 0], closestHit_ind[:, 1], closestHit_ind[:, 2]]
    print('hitValues', hitValues)
    print('hitValues.shape', hitValues.shape)
    p = np.exp(-distances)
    print('p', p)
    print('p.shape', p.shape)
    print('grid_copy.shape', grid_copy.shape)
    print('p * hitValues', p * hitValues)
    print('(p * hitValues).shape', (p * hitValues).shape)
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

    for start, end, weight in zip(starts, ends, hitWeights):
        grid += get_voxelGrid_hitVector(shape, start, end, weight, verbose=verbose)

    if make_errors:
        grid = get_voxelGrid_errors(grid)#, max_distance=max_distance)

    grid_ind = np.argwhere(grid >= 0)
    grid_pos = grid_ind / scale - detectorDimensions[np.newaxis, :] / 2

    return grid, grid_ind, grid_pos