import tqdm
import numpy as np
import pandas as pd

def make_reconstructedVertex(df_hits, grid_function, grid_size, detector_size_mm, make_errors, type='mean'):
    recoVertex = np.ones((len(df_hits), 3))*-1
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
            recoVertex[inds] = np.array([np.nan, np.nan, np.nan])
        else:
            if type == 'mean':
                recoVertex[inds] = np.array([
                    np.average(np.unique(grid_pos[:,0]), weights=np.mean(grid, axis=(1,2))),
                    np.average(np.unique(grid_pos[:,1]), weights=np.mean(grid, axis=(0,2))),
                    np.average(np.unique(grid_pos[:,2]), weights=np.mean(grid, axis=(0,1)))
                ])
            elif type == 'max':
                # print('inds', inds)
                # print('np.reshape(grid, (-1,))', np.reshape(grid, (-1,)))
                # print('np.argmax(np.reshape(grid, (-1,)))', np.argmax(np.reshape(grid, (-1,))))
                # print('grid_pos[np.argmax(np.reshape(grid, (-1,)))]', grid_pos[np.argmax(np.reshape(grid, (-1,)))])
                # print()
                recoVertex[inds] = grid_pos[np.argmax(np.reshape(grid, (-1,)))]
            else:
                raise ValueError('type must be "mean" or "max"')
    
    if type == 'mean':
        df_hits['reconstructedVertex_mean'] = recoVertex.tolist()
    elif type == 'max':
        df_hits['reconstructedVertex_max'] = recoVertex.tolist()

    return df_hits