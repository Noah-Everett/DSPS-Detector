import ROOT
import uproot
import numpy as np
import pandas as pd
import awkward as ak

def get_histogram_names(fileName, directoryName=None, fullPath=False):
    file = uproot.open(fileName)
    keys = file.keys()
    if directoryName is not None:
        keys = file[directoryName].keys()
    file.close()

    if fullPath:
        if directoryName is None:
            directoryName = ''
        if not directoryName.endswith('/'):
            directoryName += '/'
        keys = [directoryName + key for key in keys]

    return keys

def get_histogram_titles(fileName, directoryName=None):
    file = ROOT.TFile(fileName)
    names = get_histogram_names(fileName, directoryName)
    names_withDir = [directoryName + '/' + name for name in names]
    titles = [file.Get(name).GetTitle() for name in names_withDir]
    file.Close()
    return titles

def get_histogram_positions(fileName, directoryName=None):
    titles = get_histogram_titles(fileName, directoryName)
    positions = [(float(title.split('_')[2]), float(title.split('_')[3]), float(title.split('_')[4])) for title in titles]
    return positions

def get_histogram_walls(fileName, directoryName=None):
    titles = get_histogram_titles(fileName, directoryName)
    walls = [title.split('_')[1] for title in titles]
    return walls

def get_tree_names(fileName, histDirectoryName=None):
    file = uproot.open(fileName)
    keys = file.keys()
    if histDirectoryName is not None:
        keys = [key for key in keys if histDirectoryName not in key]
    file.close()
    return keys

def get_photosensor_hits_position_relative(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    photosensor_relative_x = tree['photoSensor_hits_position_relative_x'].array()
    photosensor_relative_y = tree['photoSensor_hits_position_relative_y'].array()
    photosensor_relative_z = tree['photoSensor_hits_position_relative_z'].array()
    file.close()
    return list(zip(photosensor_relative_x, photosensor_relative_y, photosensor_relative_z))

def get_photosensor_hits_position_absolute(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    photosensor_absolute_x = tree['photoSensor_hits_position_absolute_x'].array()
    photosensor_absolute_y = tree['photoSensor_hits_position_absolute_y'].array()
    photosensor_absolute_z = tree['photoSensor_hits_position_absolute_z'].array()
    file.close()
    return list(zip(photosensor_absolute_x, photosensor_absolute_y, photosensor_absolute_z))

import numpy as np

def get_photosensor_hits_position_relative_bins(fileName, treeName, histDirectoryName):
    file = uproot.open(fileName)
    tree = file[treeName]

    photosensor_relative_x = tree['photoSensor_hits_position_relative_x'].array()
    photosensor_relative_y = tree['photoSensor_hits_position_relative_y'].array()

    histogram = file[get_histogram_names(fileName, histDirectoryName, fullPath=True)[0]]

    x_bins = histogram.axis(0).edges()
    y_bins = histogram.axis(1).edges()

    x_edges = np.linspace(x_bins.min(), x_bins.max(), len(x_bins))
    y_edges = np.linspace(y_bins.min(), y_bins.max(), len(y_bins))

    x_edges[0] -= 1e-9
    y_edges[0] -= 1e-9
    x_edges[-1] += 1e-9
    y_edges[-1] += 1e-9

    position_relative_x_bins = pd.Series(pd.cut(np.array(photosensor_relative_x), bins=x_edges, retbins=False))
    position_relative_y_bins = pd.Series(pd.cut(np.array(photosensor_relative_y), bins=y_edges, retbins=False))

    file.close()
    return position_relative_x_bins, position_relative_y_bins

def get_photosensor_hits_position_relative_binned(fileName, treeName, histDirectoryName):
    position_relative_x_bins, position_relative_y_bins = get_photosensor_hits_position_relative_bins(fileName, treeName, histDirectoryName)

    position_relative_x_binned = position_relative_x_bins.apply(lambda x: (x.right + x.left) / 2)
    position_relative_y_binned = position_relative_y_bins.apply(lambda x: (x.right + x.left) / 2)

    return list(zip(position_relative_x_binned, position_relative_y_binned))

def get_photosensor_hits_position_relative_nBin(fileName, treeName, histDirectoryName):
    position_relative_x_bins, position_relative_y_bins = get_photosensor_hits_position_relative_bins(fileName, treeName, histDirectoryName)

    return list(zip(position_relative_x_bins.cat.codes, position_relative_y_bins.cat.codes))

def get_photosensor_hits_position_initial(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    x = tree['photoSensor_hits_position_initial_x'].array()
    y = tree['photoSensor_hits_position_initial_y'].array()
    z = tree['photoSensor_hits_position_initial_z'].array()
    file.close()
    return list(zip(x, y, z))

def get_photosensor_hits_time(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    hit_time = tree['photoSensor_hits_time'].array()
    file.close()
    return hit_time

def get_photosensor_hits_photosensor_ID(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    photosensor_id = tree['photoSensor_hits_photoSensorID'].array()
    file.close()
    return photosensor_id

def get_photosensor_hits_photosensor_position(fileName, treeName):
    IDs = get_photosensor_hits_photosensor_ID(fileName, treeName)
    positions = [(float(ID.split('_')[2]), float(ID.split('_')[3]), float(ID.split('_')[4])) for ID in IDs]
    return positions

def get_photosensor_hits_photosensor_wall(fileName, treeName):
    IDs = get_photosensor_hits_photosensor_ID(fileName, treeName)
    walls = [ID.split('_')[1] for ID in IDs]
    return walls

def get_photosensor_hits_photosensor_direction(fileName, treeName):
    walls = get_photosensor_hits_photosensor_wall(fileName, treeName)

    directions = []
    for wall in walls:
        if wall == '+x':
            directions.append([-1, 0, 0])
        elif wall == '-x':
            directions.append([+1, 0, 0])
        elif wall == '+y':
            directions.append([0, -1, 0])
        elif wall == '-y':
            directions.append([0, +1, 0])
        elif wall == '+z':
            directions.append([0, 0, -1])
        elif wall == '-z':
            directions.append([0, 0, +1])

    return directions

def get_primary_position(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    x = tree['primary_position_x'].array()
    y = tree['primary_position_y'].array()
    z = tree['primary_position_z'].array()
    file.close()
    return list(zip(x, y, z))

def get_primary_time(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    time = tree['primary_time'].array()
    file.close()
    return time

def get_photosensor_hits_momentum(filename, treeName):
    file = uproot.open(filename)
    tree = file[treeName]
    px = tree['photoSensor_hits_momentum_x'].array()
    py = tree['photoSensor_hits_momentum_y'].array()
    pz = tree['photoSensor_hits_momentum_z'].array()
    file.close()
    return list(zip(px, py, pz))

def get_photosensor_hits_energy(filename, treeName):
    file = uproot.open(filename)
    tree = file[treeName]
    energy = tree['photoSensor_hits_energy'].array()
    file.close()
    return energy

def get_photosensor_hits_direction_relative(filename, treeName):
    file = uproot.open(filename)
    tree = file[treeName]
    x = tree['photoSensor_hits_direction_relative_x'].array()
    y = tree['photoSensor_hits_direction_relative_y'].array()
    z = tree['photoSensor_hits_direction_relative_z'].array()
    file.close()
    return list(zip(x, y, z))

def get_lens_hits_direction_relative(filename, treeName, lens="0"):
    file = uproot.open(filename)
    tree = file[treeName]
    x = tree['lens_hits_direction_relative_x'].array()
    y = tree['lens_hits_direction_relative_y'].array()
    z = tree['lens_hits_direction_relative_z'].array()
    n = tree['lens_hits_lensID'].array()
    file.close()
    print(n)
    # return list(zip(x, y, z))
    return n

def get_photosensor_hits_direction_relative_lens(filename, treeName, nLens=0):
    file = uproot.open(filename)
    tree = file[treeName]
    x = tree['photoSensor_hits_direction_relative_lens_{}_x'.format(nLens)].array()
    y = tree['photoSensor_hits_direction_relative_lens_{}_y'.format(nLens)].array()
    z = tree['photoSensor_hits_direction_relative_lens_{}_z'.format(nLens)].array()
    file.close()
    return list(zip(x, y, z))

def get_photosensor_hits_position_relative_lens(filename, treeName, nLens=0):
    file = uproot.open(filename)
    tree = file[treeName]
    x = tree['photoSensor_hits_position_relative_lens_{}_x'.format(nLens)].array()
    y = tree['photoSensor_hits_position_relative_lens_{}_y'.format(nLens)].array()
    z = tree['photoSensor_hits_position_relative_lens_{}_z'.format(nLens)].array()
    file.close()
    return list(zip(x, y, z))