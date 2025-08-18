import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import PathPatch
from matplotlib.path import Path
from matplotlib import colors


def hatch_between(bins, hist, y=0, ax=None, **kwargs):
    """
    Draw a hatched, step-shaped polygon that follows a histogram outline.

    Parameters
    ----------
    bins : array-like
        The bin edges (length N+1). This should match the output of np.histogram(...)[1].
    hist : array-like
        The histogram bin heights (length N). This should match np.histogram(...)[0].
    y : float, default 0
        Baseline y-value to close the polygon (typically 0).
    ax : matplotlib.axes.Axes or None
        Axes to draw on. If None, use the current axes (plt.gca()).
    **kwargs
        Forwarded to matplotlib.patches.PathPatch. Common options:
        edgecolor, facecolor, linewidth, hatch, label, alpha, etc.

    Notes
    -----
    - This function mirrors the "manual step" approach used in your reference:
      it duplicates edges/heights to create a proper step, then constructs a
      Path/PathPatch so you can apply hatching and have a patch object for legend.
    - It does NOT compute the histogram—only draws from (bins, hist).

    Returns
    -------
    None
        (Patch is added to the axes.)
    """
    if ax is None:
        ax = plt.gca()

    # Duplicate bar heights and edges to form a "step" polygon and close it to baseline y
    hist = np.insert(hist.repeat(2), [0, len(hist) * 2], y)
    bins = np.array(bins).repeat(2)

    path = Path(np.array([bins, hist]).T)
    patch = PathPatch(path, **kwargs)

    ax.add_patch(patch)


def add_step_hist(
    data,
    bins=50,
    hist_range=None,
    density=True,
    ax=None,
    baseline=0.0,
    linewidth=2,
    edgecolor=None,
    facecolor='None',
    hatch=None,
    label=None,
    alpha=1.0,
):
    """
    Compute a histogram and render it as a hatched, step-shaped polygon.

    This replicates the "hist -> repeat -> Path -> PathPatch" pattern from your
    reference plotting code, enabling hatching, patch-style legends, and
    separate control over edge vs. face (unlike `histtype='step'`, which draws
    only a Line2D and cannot be hatched).

    Parameters
    ----------
    data : array-like
        1D data to histogram.
    bins : int or sequence, default 50
        Histogram bins (passed to np.histogram).
    hist_range : (float, float) or None
        Lower and upper range of the bins (passed to np.histogram via 'range').
        Named 'hist_range' to avoid shadowing Python's built-in 'range'.
    density : bool, default True
        If True, normalize the histogram to form a probability density.
    ax : matplotlib.axes.Axes or None
        Axes to draw on. If None, uses current axes via plt.gca().
    baseline : float, default 0.0
        Baseline y-value used to "close" the step polygon.
    linewidth : float, default 2
        Edge linewidth of the step polygon.
    edgecolor : color or None
        Edge color of the step polygon.
    facecolor : color or 'None', default 'None'
        Face color of the polygon (keep 'None' for outline-only).
    hatch : str or None
        Matplotlib hatch pattern (e.g., '//', '\\\\', 'xx', '...').
    label : str or None
        Legend label for the polygon.
    alpha : float, default 1.0
        Patch alpha.

    Returns
    -------
    counts : np.ndarray
        Histogram counts (or densities if density=True), length = len(bins)-1 when bins is array-like.
    edges : np.ndarray
        Bin edges from np.histogram.
    patch : matplotlib.patches.PathPatch
        The patch added to the axes.

    Notes
    -----
    - Use this when you want the aesthetic and legend behavior of patches,
      including hatching, rather than a simple outline produced by
      `ax.hist(..., histtype='step')`.
    - If you already have (counts, edges), call `hatch_between(edges, counts, ...)`
      directly instead of recomputing the histogram here.
    """
    if ax is None:
        ax = plt.gca()

    # Compute the histogram (no plotting). We name the 'range' argument 'hist_range' to avoid
    # confusion with Python's built-in 'range'.
    counts, edges = np.histogram(data, bins=bins, range=hist_range, density=density)

    # Build the “step” arrays and create a closed polygon path
    counts_step = np.insert(counts.repeat(2), [0, len(counts) * 2], baseline)
    edges_step = np.array(edges).repeat(2)

    path = Path(np.column_stack([edges_step, counts_step]))
    patch = PathPatch(
        path,
        facecolor=facecolor,
        edgecolor=edgecolor,
        linewidth=linewidth,
        hatch=hatch,
        label=label,
        alpha=alpha,
    )
    ax.add_patch(patch)

    return counts, edges, patch


def plot_grid(ax, xEdges, yEdges, zEdges,
              recoGrid=None, trueGrid=None, truePoints=None,
              recoGridFaceColors=None, trueGridFaceColors=None, nullGridFaceColors='lightgray',
              recoGridEdgeColors=None, trueGridEdgeColors=None, nullGridEdgeColors='gray',
              recoGridAlpha=0.5, trueGridAlpha=0.5, nullGridAlpha=0.5, linewidth=0.5,
              truePointsColor='r', truePointsSize=10, truePointsAlpha=1, truePointsMarker='o',
              cbar=False, colorNorm=None, cmap=None,
              ignoreOuterNVoxels=0):
    """
    Plot a 3D voxel grid with optional reconstructed/true volumes and true point markers.

    This utility wraps matplotlib's Axes3D.voxels call, with optional masking of
    an outer shell of voxels, and an optional colorbar for mapped facecolors.

    Parameters
    ----------
    ax : mpl_toolkits.mplot3d.axes3d.Axes3D
        3D axes to draw on (created by e.g. fig.add_subplot(111, projection='3d')).
    xEdges, yEdges, zEdges : array-like
        Bin edges along x, y, z (as required by ax.voxels).
    recoGrid, trueGrid : ndarray or None
        Boolean (or 0/1) occupancy grids indicating which voxels to draw for
        the reconstructed and true volumes, respectively.
    truePoints : ndarray (N, 3) or None
        Optional set of (x, y, z) points to scatter (e.g., ground-truth points).
    *_FaceColors, *_EdgeColors : ndarray-like or color spec
        Face/edge colors for each voxel grid (reco, true, null).
        Can be single colors or arrays matching the voxel shapes.
    *_Alpha : float
        Transparency for each grid.
    linewidth : float
        Edge linewidth for voxels.
    truePointsColor, truePointsSize, truePointsAlpha, truePointsMarker
        Styling for the true points scatter plot.
    cbar : bool
        If True, draw a colorbar using the provided 'colorNorm' and 'cmap'.
    colorNorm : matplotlib.colors.Normalize or None
        Normalization object for the colorbar.
    cmap : matplotlib colormap or None
        Colormap for the colorbar.
    ignoreOuterNVoxels : int
        Number of voxels to blank out from each face of the volume (useful to
        hide boundary artifacts).

    Returns
    -------
    ax : mpl_toolkits.mplot3d.axes3d.Axes3D
        The axes with the plotted content.
    """
    # Plot the grid
    if recoGrid is not None:
        # set colors to None for outer n voxels
        recoGridFaceColors_ignoreOuter = recoGridFaceColors.copy()
        recoGridEdgeColors_ignoreOuter = recoGridEdgeColors.copy()

        def ignoreOuterVoxels(grid, nVoxels):
            if nVoxels == 0:
                return grid

            grid[: nVoxels, :, :] = 0
            grid[-nVoxels:, :, :] = 0
            grid[:, : nVoxels, :] = 0
            grid[:, -nVoxels:, :] = 0
            grid[:, :, : nVoxels] = 0
            grid[:, :, -nVoxels:] = 0

            return grid

        recoGridFaceColors_ignoreOuter = ignoreOuterVoxels(recoGridFaceColors_ignoreOuter, ignoreOuterNVoxels)
        recoGridEdgeColors_ignoreOuter = ignoreOuterVoxels(recoGridEdgeColors_ignoreOuter, ignoreOuterNVoxels)
        recoGrid_ignoreOuter = ignoreOuterVoxels(recoGrid, ignoreOuterNVoxels)

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
            truePoints[:, 0],
            truePoints[:, 1],
            truePoints[:, 2],
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
        np.ones((xEdges.shape[0] - 1, yEdges.shape[0] - 1, zEdges.shape[0] - 1)),
        facecolors= nullGridFaceColors,
        edgecolors= nullGridEdgeColors,
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