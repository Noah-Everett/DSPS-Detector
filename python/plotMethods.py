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
