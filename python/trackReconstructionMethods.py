import numpy as np
import tqdm
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.ndimage import rotate

from constants import *

def get_reconstructedPosition_slice(minMax, grid, grid_ind, grid_pos, axis, 
                                    detector_size_1_mm=DETECTOR_SIZE_X_MM, detector_size_2_mm=DETECTOR_SIZE_Y_MM,
                                    saveAnimation=False, plotSlice=None, makeAnimation=None, saveDir=None, showPlots=False, savePlots=False):
    if minMax != 'mean' and minMax != 'max':
        raise ValueError('minMax must be "mean" or "max"')
    
    grid_ind_tensor = np.reshape(grid_ind, grid.shape)
    grid_pos_tensor = np.reshape(grid_pos, grid.shape)

 
    pos_reco = np.ones((grid.shape[axis], 3)) * np.nan
    for i in tqdm.tqdm(np.arange(grid.shape[axis])):
        pos_reco[i,axis] = np.unique(grid_pos[:,axis])[i]

        if minMax == 'mean':
            val_1 = np.mean(grid.take(i, axis=axis), axis=0)
            val_2 = np.mean(grid.take(i, axis=axis), axis=1)

            if np.sum(val_1) == 0 or np.sum(val_2) == 0:
                continue

            pos = grid_pos[np.where(grid_ind[:,axis] == i)][:,axes]
            pos_reco[i,axes[0]] = np.average(np.unique(pos[:,0]), weights=val_1)
            pos_reco[i,axes[1]] = np.average(np.unique(pos[:,1]), weights=val_2)

        elif minMax == 'max':
            pos = grid_pos[np.where(grid_ind[:,axis] == i)][:,np.flip(axes)]
            p = pos[np.argmax(np.reshape(grid.take(i, axis=axis), -1))]
            pos_reco[i,axes[0]] = p[0]
            pos_reco[i,axes[1]] = p[1]

        if saveAnimation:
            plotSlice(grid, pos_reco[:,axes], i, axis, 
                      detector_size_1_mm, detector_size_2_mm, 
                      show=showPlots, save=savePlots, 
                      saveName='{}/{}_{}.pdf'.format(saveDir, axis, i))

    if saveAnimation:
        makeAnimation(grid, pos_reco[:,axes], axis, 
                      detector_size_1_mm, detector_size_2_mm, 
                      save=savePlots, saveName='{}/{}.gif'.format(saveDir, axis))

    return pos_reco

def plotSlice(grid, pos, i, axis, 
              size_1, size_2, 
              ax=None, show=True, 
              save=False, saveName='test.png'):
    if ax is None:
        fig, ax = plt.subplots()

    if axis == 0:
        slice = grid[i,:,:]
    elif axis == 1:
        slice = grid[:,i,:]
    elif axis == 2:
        slice = grid[:,:,i]

    imshow = ax.imshow(slice, cmap='viridis', extent=[-size_1/2, size_1/2, size_2/2, -size_2/2])
    ax.scatter(pos[i,0], pos[i,1], c='r', marker='o', s=20)
    cbar = ax.figure.colorbar(imshow, ax=ax, label='Color Bar (Log Scale)')

    if show:
        plt.show()
    if save:
        plt.savefig(saveName)

    plt.close(fig)

def makeAnimation(grid, pos, axis, size_1, size_2, save=False, saveName='test.gif'):
    fig, ax = plt.subplots()

    imshow = ax.imshow(getSlice(0), cmap='viridis', extent=[-size_1/2, size_1/2, size_2/2, -size_2/2])
    scatter = ax.scatter(pos[0,0], pos[0,1], c='r', marker='o', s=20)
    cbar = ax.figure.colorbar(imshow, ax=ax)

    def update(i):
        slice = getSlice(i)

        imshow.set_data(slice)
        scatter.set_offsets(pos[i,:])
            
        return imshow, scatter
        
    anim = FuncAnimation(fig, update, frames=np.arange(1, grid.shape[0]-1), interval=100)
    anim.save(saveName, dpi=80, writer='imagemagick')
    return anim

def getSlice_indices(grid_ind, axis, i):
    grid_ind_tensor = np.reshape(grid_ind, grid.shape)

    if axis == 0:
        slice = grid[i,:,:]
    elif axis == 1:
        slice = grid[:,i,:]
    elif axis == 2:
        slice = grid[:,:,i]

    return slice

def getSlice_axesAndRotation(axis, grid_ind):
    if axis > 2:
        grid_ind = rotate(grid_ind, 45, axes=(0,1), reshape=False)
        grid_ind = rotate(grid_ind, 45, axes=(1,2), reshape=False)
        axis -= 3

    if axis == 0:
        axes = [1,2]
    elif axis == 1:
        axes = [0,2]
    elif axis == 2:
        axes = [0,1]
    else:
        raise ValueError('axis must be 0, 1, 2, 3, 4, or 5')

    return axes

    # if axis > 2:
    #     grid_ind = rotate(grid_ind, 45, axes=(0,1), reshape=False)
    #     grid_ind = rotate(grid_ind, 45, axes=(1,2), reshape=False)
    #     axis -= 3

    # if axis == 0:
    #     axes = [1,2]
    # elif axis == 1:
    #     axes = [0,2]
    # elif axis == 2:
    #     axes = [0,1]
    # else:
    #     raise ValueError('axis must be 0, 1, 2, 3, 4, or 5')
