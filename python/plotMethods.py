import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch
from matplotlib.path import Path

def hatch_between(bins, hist, y=0, ax=None, **kwargs):
    if ax is None:
        ax = plt.gca()

    hist = np.insert(hist.repeat(2), [0, len(hist) * 2], y)
    bins = np.array(bins).repeat(2)

    path = Path(np.array([bins, hist]).T)
    patch = PathPatch(path, **kwargs)

    ax.add_patch(patch)

def plot_grid(ax, xEdges, yEdges, zEdges,
              recoGrid=None, trueGrid=None, truePoints=None,
              recoGridFaceColors=None, trueGridFaceColors=None, nullGridFaceColors='lightgray',
              recoGridEdgeColors=None, trueGridEdgeColors=None, nullGridEdgeColors='gray',
              recoGridAlpha=0.5, trueGridAlpha=0.5, nullGridAlpha=0.5, linewidth=0.5,
              truePointsColor='r', truePointsSize=10, truePointsAlpha=1, truePointsMarker='o',
              cbar=False, colorNorm=None, cmap=None,
              ignoreOuterNVoxels=0):
    # Plot the grid
    if recoGrid is not None:
        # set colors to None for outer n voxels
        recoGridFaceColors_ignoreOuter = recoGridFaceColors.copy()
        recoGridEdgeColors_ignoreOuter = recoGridEdgeColors.copy()

        def ignoreOuterVoxels(grid, nVoxels):
            if nVoxels == 0:
                return grid

            grid[: nVoxels,:,:] = 0
            grid[-nVoxels:,:,:] = 0
            grid[:,: nVoxels,:] = 0
            grid[:,-nVoxels:,:] = 0
            grid[:,:,: nVoxels] = 0
            grid[:,:,-nVoxels:] = 0

            return grid
        
        recoGridFaceColors_ignoreOuter = ignoreOuterVoxels(recoGridFaceColors_ignoreOuter, ignoreOuterNVoxels)
        recoGridEdgeColors_ignoreOuter = ignoreOuterVoxels(recoGridEdgeColors_ignoreOuter, ignoreOuterNVoxels)
        recoGrid_ignoreOuter           = ignoreOuterVoxels(recoGrid                      , ignoreOuterNVoxels)

        recoGridVoxels = ax.voxels(
            xEdges, 
            yEdges, 
            zEdges,
            recoGrid_ignoreOuter,
            facecolors=recoGridFaceColors_ignoreOuter,
            edgecolors=recoGridEdgeColors_ignoreOuter,
            alpha=recoGridAlpha,
            linewidth=linewidth
        )
    if trueGrid is not None:
        ax.voxels(
            xEdges, 
            yEdges, 
            zEdges,
            trueGrid,
            facecolors=trueGridFaceColors,
            edgecolors=trueGridEdgeColors,
            alpha=trueGridAlpha,
            linewidth=linewidth
        )
    if truePoints is not None:
        ax.scatter(
            truePoints[:,0], 
            truePoints[:,1], 
            truePoints[:,2], 
            color=truePointsColor,
            marker=truePointsMarker,
            s=truePointsSize,
            alpha=truePointsAlpha
        )

    # Plot the null grid
    ax.voxels(
        xEdges, 
        yEdges, 
        zEdges,
        np.ones((xEdges.shape[0]-1, yEdges.shape[0]-1, zEdges.shape[0]-1)),
        facecolors=nullGridFaceColors,
        edgecolors=nullGridEdgeColors,
        alpha=nullGridAlpha,
        linewidth=linewidth
    )

    # Add colorbar
    if cbar and colorNorm is not None and cmap is not None:
        sm = plt.cm.ScalarMappable(cmap=cmap, norm=colorNorm)
        plt.colorbar(sm, ax=ax, shrink=0.6)

    ax.set_axis_off()
    ax.set_aspect('equal')

    return ax
