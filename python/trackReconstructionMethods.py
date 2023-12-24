import numpy as np
import tqdm

def get_voxelGrid_hitVector(shape, start, end, weight):
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

    if dx >= dy and dx >= dz:
        # X is the dominant axis
        d1 = dy - dx
        d2 = dz - dx
        while x1 != x2:
            grid[x1, y1, z1] = weight
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
            grid[x1, y1, z1] = weight
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
            grid[x1, y1, z1] = weight
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
    grid[x2, y2, z2] = weight
    return grid

def get_voxelGrid_errors(grid, max_distance=10):
    grid_copy = grid.copy()

    points = np.argwhere(grid_copy >= 0).tolist()
    points = [tuple(point) for point in points]

    points_hit = np.argwhere(grid_copy > 0).tolist()
    points_hit = [tuple(point) for point in points_hit]
    
    for i in tqdm.tqdm(range(max_distance+1)):
        p = np.exp(-i)
        
        for point in points_hit:
            x, y, z = point
            
            # Generate all possible new points within the distance
            new_points = [(x + dx, y + dy, z + dz) for dx in range(-i, i + 1)
                          for dy in range(-i, i + 1) 
                          for dz in range(-i, i + 1)]
            
            # Filter new points that are in the list of existing points
            valid_new_points = filter(lambda np: np in points, new_points)
            valid_new_points = [(x, y, z) for x, y, z in valid_new_points if (x, y, z) != point]

            
            # Update grid_copy for valid new points
            for new_point in valid_new_points:
                if grid_copy[new_point] < p * grid_copy[point]:
                    grid_copy[new_point] = p * grid_copy[point]

    return grid_copy

def get_voxelGrid(shape, starts, ends, hitWeights=None, max_distance=10, make_errors=False):
    grid = np.zeros(shape, dtype=float)

    for start, end, weight in zip(starts, ends, hitWeights):
        grid += get_voxelGrid_hitVector(shape, start, end, weight)

    if make_errors:
        grid = get_voxelGrid_errors(grid, max_distance=max_distance)

    return grid