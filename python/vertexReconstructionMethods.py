import tqdm
import numpy as np
import pandas as pd

def make_reconstructedVertex(df_hits, grid_function, grid_size, detector_size_mm, make_errors):
    recoVertex = np.ones(df_hits['initialPosition'].shape) * -1
    for initialPosition in tqdm.tqdm(np.unique(df_hits['initialPosition'].to_numpy())):
        inds = df_hits['initialPosition'] == initialPosition
        grid, grid_ind, grid_pos = grid_function(
            grid_size,
            detector_size_mm,
            np.array(df_hits['sensor_position'][inds].to_list()).reshape(-1,3),
            np.array(df_hits['reconstructedVector_direction'][inds].to_list()).reshape(-1,3),
            [1 for i in range(len(df_hits['sensor_position'][inds]))],
            20,
            make_errors
        )
        if np.sum(grid, axis=0).max() == 0 or np.sum(grid, axis=1).max() == 0 or np.sum(grid, axis=2).max() == 0:
            continue
        else:
            recoVertex[inds] = np.array([
                np.average(np.unique(grid_pos[:,0]), weights=np.mean(grid, axis=(1,2))),
                np.average(np.unique(grid_pos[:,1]), weights=np.mean(grid, axis=(0,2))),
                np.average(np.unique(grid_pos[:,2]), weights=np.mean(grid, axis=(0,1)))
            ])
    
    df_hits['reconstructedVertex'] = recoVertex
    return df_hits