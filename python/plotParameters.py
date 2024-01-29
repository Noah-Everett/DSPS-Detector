import os
import matplotlib as mpl

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