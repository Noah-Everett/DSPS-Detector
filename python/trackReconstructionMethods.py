import numpy as np
import tqdm
import pandas as pd

from constants import *

def get_reconstructedPosition_slice_mean(grid, grid_ind, grid_pos, axis, 
                                         detector_size_1_mm=DETECTOR_SIZE_X_MM, detector_size_2_mm=DETECTOR_SIZE_Y_MM,
                                         saveAnimation=False, plotSlice=None, makeAnimation=None):
    if axis == 0:
        axes = [1,2]
    elif axis == 1:
        axes = [0,2]
    elif axis == 2:
        axes = [0,1]
    else:
        raise ValueError('axis must be 0, 1 or 2')

    pos_avg = np.zeros((grid.shape[axis], 3))
    for i in tqdm.tqdm(np.arange(grid.shape[axis])):
        pos_avg[i,axis] = np.unique(grid_pos[:,axis])[i]

        val_1 = np.mean(grid.take(i, axis=axis), axis=0)
        val_2 = np.mean(grid.take(i, axis=axis), axis=1)

        if np.sum(val_1) == 0 or np.sum(val_2) == 0:
            continue
        
        pos = grid_pos[np.where(grid_ind[:,axis] == i)][:,axes]

        pos_avg[i,axes[0]] = np.average(np.unique(pos[:,0]), weights=val_1)
        pos_avg[i,axes[1]] = np.average(np.unique(pos[:,1]), weights=val_2)

        if saveAnimation:
            plotSlice(grid, pos_avg, i, axis, detector_size_1_mm, detector_size_2_mm, show=showPlots, save=savePlots, saveName='{}/{}_{}.pdf'.format(saveDir, axis, i))

    if saveAnimation:
        makeAnimation(grid, pos_avg, axis, detector_size_1_mm, detector_size_2_mm, save=savePlots, saveName='{}/{}.gif'.format(saveDir, axis))

    return pos_avg







# pos_avg = np.zeros((grid.shape[0], 2))
# for x in np.arange(grid.shape[0]):
#     val_y = np.mean(grid[x,:,:], axis=0)
#     val_z = np.mean(grid[x,:,:], axis=1)
#     if np.sum(val_y) == 0 or np.sum(val_z) == 0:
#         continue
#     pos = grid_pos[np.where(grid_ind[:,0] == x)][:,1:3]
#     pos_avg[x,0] = np.average(np.unique(pos[:,0]), weights=val_y)
#     pos_avg[x,1] = np.average(np.unique(pos[:,1]), weights=val_z)
#     plotSlice(grid, pos_avg, x, 0, DETECTOR_SIZE_Y_MM, DETECTOR_SIZE_Z_MM, show=showPlots, save=savePlots, saveName='{}/x_{}.pdf'.format(saveDir, x))
# if saveAnima:
#     makeAnimation(grid, pos_avg, 0, DETECTOR_SIZE_Y_MM, DETECTOR_SIZE_Z_MM, save=savePlots, saveName='{}/x.gif'.format(saveDir))
# X_mean = np.array([np.unique(grid_pos[:,0]), pos_avg[:,0], pos_avg[:,1]]).T

# pos_avg = np.zeros((grid.shape[1], 2))
# for y in np.arange(grid.shape[1]):
#     val_x = np.mean(grid[:,y,:], axis=0)
#     val_z = np.mean(grid[:,y,:], axis=1)
#     if np.sum(val_x) == 0 or np.sum(val_z) == 0:
#         continue
#     pos = grid_pos[np.where(grid_ind[:,1] == y)][:,[0,2]]
#     pos_avg[y,0] = np.average(np.unique(pos[:,0]), weights=val_x)
#     pos_avg[y,1] = np.average(np.unique(pos[:,1]), weights=val_z)
#     plotSlice(grid, pos_avg, y, 1, DETECTOR_SIZE_X_MM, DETECTOR_SIZE_Z_MM, show=showPlots, save=savePlots, saveName='{}/y_{}.pdf'.format(saveDir, y))
# if saveAnima:
#     makeAnimation(grid, pos_avg, 1, DETECTOR_SIZE_X_MM, DETECTOR_SIZE_Z_MM, save=savePlots, saveName='{}/y.gif'.format(saveDir))
# Y_mean = np.array([pos_avg[:,0], np.unique(grid_pos[:,1]), pos_avg[:,1]]).T

# pos_avg = np.zeros((grid.shape[2],2))
# for z in np.arange(grid.shape[2]):
#     val_x = np.mean(grid[:,:,z], axis=0)
#     val_y = np.mean(grid[:,:,z], axis=1)
#     if np.sum(val_x) == 0 or np.sum(val_y) == 0:
#         continue
#     pos = grid_pos[np.where(grid_ind[:,2] == z)][:,:2]
#     pos_avg[z,0] = np.average(np.unique(pos[:,0]), weights=val_x)
#     pos_avg[z,1] = np.average(np.unique(pos[:,1]), weights=val_y)
#     plotSlice(grid, pos_avg, z, 2, DETECTOR_SIZE_X_MM, DETECTOR_SIZE_Y_MM, show=showPlots, save=savePlots, saveName='{}/z_{}.pdf'.format(saveDir, z))
# if saveAnima:
#     makeAnimation(grid, pos_avg, 2, DETECTOR_SIZE_X_MM, DETECTOR_SIZE_Y_MM, save=savePlots, saveName='{}/z.gif'.format(saveDir))
# Z_mean = np.array([pos_avg[:,0], pos_avg[:,1], np.unique(grid_pos[:,2])]).T

# avg_mean = np.mean(np.array([X_mean,Y_mean,Z_mean]), axis=0)