import os
import matplotlib as mpl
import numpy as np
import matplotlib.cm as cm
from matplotlib.colors import ListedColormap

# Set the path to the TeX binaries
os.environ['PATH'] = '/Library/TeX/texbin:' + os.environ['PATH']

# Set the default font to Computer Modern Roman
mpl.rc(
    'font', 
    **{
        'family':'serif', 
        'serif' :['Computer Modern Roman']
    }
)

# Set the default parameters for the plots
params = {
    'backend'            : 'pdf'                 ,
    'axes.labelsize'     : 20                    ,
    'font.size'          : 20                    ,
    'legend.fontsize'    : 20                    ,
    'xtick.labelsize'    : 20                    ,
    'ytick.labelsize'    : 20                    ,
    'text.usetex'        : True                  ,
    'text.latex.preamble': r"""
        \usepackage{amsbsy}
        \usepackage{amsmath}

        \providecommand{\abs}[1]{\lvert#1\rvert} 
        \providecommand{\norm}[1]{\lVert#1\rVert}
    """                                          ,
    'figure.figsize'     : (5,5)                 ,
    'savefig.directory'  : '../figures'          ,
    'axes.unicode_minus' : True
}
mpl.rcParams.update(params)

rainbow = cm.rainbow(np.linspace(0, 1, 10))
rainbow = np.insert(rainbow, 0, [0, 0, 0, 1], axis=0)
rainbow_cmap = ListedColormap(rainbow)