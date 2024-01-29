import tqdm
import numpy as np
import pandas as pd

def get_reconstructedVertex(meanMax, 
    grid=None, grid_ind=None, grid_pos=None,
    df_hits=None, grid_function=None, grid_size=None, detector_size_mm=None, make_errors=False):

    if meanMax is not 'mean' and meanMax is not 'max':
        raise ValueError('meanMax must be "mean" or "max"')

    if grid is None or grid_ind is None or grid_pos is None:
        grid, grid_ind, grid_pos = grid_function(
            grid_size,
            detector_size_mm,
            np.array(df_hits['sensor_position'].to_list()).reshape(-1,3),
            np.array(df_hits['reconstructedVector_direction'].to_list()).reshape(-1,3),
            [1 for i in range(len(df_hits['sensor_position']))],
            20,
            make_errors
        )

    if np.sum(grid, axis=0).max() == 0 or np.sum(grid, axis=1).max() == 0 or np.sum(grid, axis=2).max() == 0:
        recoVertex = np.array([np.nan, np.nan, np.nan])
    else:
        if meanMax == 'mean':
            recoVertex = np.array([
                np.average(np.unique(grid_pos[:,0]), weights=np.mean(grid, axis=(1,2))),
                np.average(np.unique(grid_pos[:,1]), weights=np.mean(grid, axis=(0,2))),
                np.average(np.unique(grid_pos[:,2]), weights=np.mean(grid, axis=(0,1)))
            ])
        elif meanMax == 'max':
            recoVertex = grid_pos[np.argmax(np.reshape(grid, (-1,)))]
        else:
            raise ValueError('meanMax must be "mean" or "max"')
    
    return recoVertex

def get_reconstructedVertex_grids(df_hits, grid_function, grid_size, detector_size_mm, make_errors=False):
    grid, grid_ind, grid_pos = grid_function(
        grid_size,
        detector_size_mm,
        np.array(df_hits['sensor_position'].to_list()).reshape(-1,3),
        np.array(df_hits['reconstructedVector_direction'].to_list()).reshape(-1,3),
        [1 for i in range(len(df_hits['sensor_position']))],
        20,
        make_errors
    )

    return grid, grid_ind, grid_pos