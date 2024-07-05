import ROOT
import uproot
import numpy as np
import pandas as pd
import awkward as ak
from constants import *
import concurrent.futures

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
    names = get_histogram_names(fileName, directoryName, fullPath=True)
    titles = [file.Get(name).GetTitle() for name in names]
    file.Close()
    return titles

def get_histogram_position(title):
    return (float(title.split('_')[2]), float(title.split('_')[3]), float(title.split('_')[4]))

def get_histogram_positions(fileName, directoryName=None):
    titles = get_histogram_titles(fileName, directoryName)
    positions = [get_histogram_position(title) for title in titles]
    return positions

def get_histogram_wall(title):
    return title.split('_')[1]

def get_histogram_walls(fileName, directoryName=None):
    titles = get_histogram_titles(fileName, directoryName)
    walls = [get_histogram_wall(title) for title in titles]
    return walls

def get_histogram_direction(title):
    wall = get_histogram_wall(title)
    if wall == '+x':
        return -1, 0, 0
    elif wall == '-x':
        return +1, 0, 0
    elif wall == '+y':
        return 0, -1, 0
    elif wall == '-y':
        return 0, +1, 0
    elif wall == '+z':
        return 0, 0, -1
    elif wall == '-z':
        return 0, 0, +1
    
def get_histogram_directions(fileName, directoryName=None):
    titles = get_histogram_titles(fileName, directoryName)
    directions = [get_histogram_direction(title) for title in titles]
    return directions

def get_histogram_sizes(fileName, directoryName=None):
    file = uproot.open(fileName)
    keys = get_histogram_names(fileName, directoryName, fullPath=True)
    sizes = [file[key].values().shape for key in keys]
    file.close()
    return sizes

def get_histogram_nHits_individual(fileName, directoryName=None, returnNames=False, unique=False):
    file = uproot.open(fileName)
    keys = get_histogram_names(fileName, directoryName, fullPath=True)

    file_values = {key: file[key].values() for key in keys}

    if unique:
        nHits = (np.count_nonzero(value) for value in file_values.values())
    else:
        nHits = (value.sum() for value in file_values.values())    

    file.close()

    if returnNames:
        return nHits, keys
    else:
        return nHits
    
def get_histogram_nHits_total(fileName, directoryName=None, unique=False):
    file = uproot.open(fileName)
    keys = get_histogram_names(fileName, directoryName, fullPath=True)

    size = get_histogram_sizes(fileName, directoryName)[0]
    tensor = np.zeros((len(keys), *size))

    for i, key in enumerate(keys):
        tensor[i] = file[key].values()

    if unique:
        nHits = np.count_nonzero(tensor)
    else:
        nHits = tensor.sum()

    file.close()

    return nHits

def get_histogram_wall(title):
    return title.split('_')[1]

def get_histogram_walls(fileName, directoryName=None):
    titles = get_histogram_titles(fileName, directoryName)
    walls = [get_histogram_wall(title) for title in titles]
    return walls

def get_histogram_hits_position_relative(fileName, directoryName=None, numpy=False):
    file = uproot.open(fileName)
    keys = get_histogram_names(fileName, directoryName, fullPath=True)
    x = [file[key].axis(0).centers() for key in keys]
    y = [file[key].axis(1).centers() for key in keys]
    file.close()

    if numpy:
        return np.array([np.array(x), np.array(y)]).T
    else:
        return list(zip(x, y))
    
def get_histogram_hits_tuple(fileName, directoryName=None, numpy=False,
                             photosensor_ID=True, photosensor_direction=True, 
                             photosensor_position=True, photosensor_wall=True, 
                             position_relative_binned=True, position_relative_nBin=True):
    file = uproot.open(fileName)
    names = get_histogram_names(fileName, directoryName, fullPath=True)
    titles = get_histogram_titles(fileName, directoryName)

    photosensor_IDs = []
    photosensor_directions = []
    photosensor_positions = []
    photosensor_walls = []
    position_relative_binneds = []
    position_relative_nBins = []

    for name, title in zip(names, titles):
        histogram = file[name]
        nHits = int(histogram.values().sum())

        if photosensor_ID:
            photosensor_IDs.extend([name] * nHits)
        if photosensor_direction:
            photosensor_directions.extend([get_histogram_direction(title)] * nHits)
        if photosensor_position:
            photosensor_positions.extend([get_histogram_position(title)] * nHits)
        if photosensor_wall:
            photosensor_walls.extend([get_histogram_wall(title)] * nHits)
        if position_relative_binned or position_relative_nBin:
            v = np.array(histogram.values())
            x = np.array(histogram.axis(0).centers()).reshape(-1)
            y = np.array(histogram.axis(1).centers()).reshape(-1)
            x, y = np.meshgrid(x, y, indexing='ij')

            if position_relative_nBin:
                posX, posY = np.where(v >= 1)
                for v_i, posX_i, posY_i in zip(v.flatten()[np.where(v.flatten() >= 1)], posX, posY):
                    position_relative_nBins.extend([[posX_i, posY_i]] * int(v_i))

            if position_relative_binned:
                v = v.flatten()
                ind = np.where(v >= 1)
                v = v[np.where(v >= 1)]
                pos = np.array([x.flatten()[ind], y.flatten()[ind]]).T
                for v_i, pos_i in zip(v, pos):
                    position_relative_binneds.extend([pos_i] * int(v_i))

    file.close()

    if numpy:
        photosensor_IDs = np.array(photosensor_IDs).astype('U'+str(max(map(len, photosensor_IDs))))
        photosensor_directions = np.array(photosensor_directions).reshape(-1, 3).astype(np.float16)
        photosensor_positions = np.array(photosensor_positions).reshape(-1, 3).astype(np.float16)
        photosensor_walls = np.array(photosensor_walls).astype('U'+str(max(map(len, photosensor_walls))))
        position_relative_binneds = np.array(position_relative_binneds).reshape(-1, 2).astype(np.float16)
        position_relative_nBins = [np.array(x).reshape(2).astype(np.int8) for x in position_relative_nBins]

    returns = []
    if photosensor_ID:
        returns.append(photosensor_IDs)
    if photosensor_direction:
        returns.append(photosensor_directions)
    if photosensor_position:
        returns.append(photosensor_positions)
    if photosensor_wall:
        returns.append(photosensor_walls)
    if position_relative_binned:
        returns.append(position_relative_binneds)
    if position_relative_nBin:
        returns.append(position_relative_nBins)

    return returns

def get_tree_names(fileName, histDirectoryName=None):
    file = uproot.open(fileName)
    keys = file.keys()
    if histDirectoryName is not None:
        keys = [key for key in keys if histDirectoryName not in key]
    file.close()
    return keys

def get_photosensor_hits_position_relative(fileName, treeName='photoSensor_hits;1', useHistograms=False, verbose=True):
    file = uproot.open(fileName)
    
    try:
        tree = file[treeName]
        photosensor_relative_x = tree['photoSensor_hits_position_relative_x'].array()
        photosensor_relative_y = tree['photoSensor_hits_position_relative_y'].array()
        photosensor_relative_z = tree['photoSensor_hits_position_relative_z'].array()
    except:
        useHistograms = True
        if verbose:
            print('No tree found')

    if useHistograms:
        if verbose:
            print('ERROR: relative position data is not available in histograms. Only relative binned data is available.')
        return None

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

def get_photosensor_hits_position_relative_bins(fileName, treeName, histDirectoryName, x=None, y=None, nBins_x=None, nBins_y=None, forDF=False):
    file = uproot.open(fileName)
    tree = file[treeName]

    if x is None or y is None:
        x = tree['photoSensor_hits_position_relative_x'].array()
        y = tree['photoSensor_hits_position_relative_y'].array()

    histogram = file[get_histogram_names(fileName, histDirectoryName, fullPath=True)[0]]

    x_bins = histogram.axis(0).edges()
    y_bins = histogram.axis(1).edges()

    if nBins_x is None:
        nBins_x = len(x_bins) - 1
    if nBins_y is None:
        nBins_y = len(y_bins) - 1

    x_edges = np.linspace(x_bins.min(), x_bins.max(), nBins_x + 1)
    y_edges = np.linspace(y_bins.min(), y_bins.max(), nBins_y + 1)

    x_edges[0] -= 1e-9
    y_edges[0] -= 1e-9
    x_edges[-1] += 1e-9
    y_edges[-1] += 1e-9

    position_relative_x_bins = pd.Series(pd.cut(np.array(x), bins=x_edges, retbins=False))
    position_relative_y_bins = pd.Series(pd.cut(np.array(y), bins=y_edges, retbins=False))

    file.close()

    if not forDF:
        return position_relative_x_bins, position_relative_y_bins
    else:
        return np.array([np.array(position_relative_x_bins), np.array(position_relative_y_bins)]).T.tolist()

def get_photosensor_hits_position_relative_binned(fileName, treeName='photoSensor_hits;1', histDirectoryName='/photoSensor_hits_histograms', x=None, y=None, nBins_x=None, nBins_y=None, verbose=True):
    position_relative_x_bins, position_relative_y_bins = get_photosensor_hits_position_relative_bins(fileName, treeName, histDirectoryName, x, y, nBins_x, nBins_y, verbose)

    position_relative_x_binned = position_relative_x_bins.apply(lambda x: (x.right + x.left) / 2)
    position_relative_y_binned = position_relative_y_bins.apply(lambda x: (x.right + x.left) / 2)

    return list(zip(position_relative_x_binned, position_relative_y_binned))

def get_photosensor_hits_position_relative_nBin(fileName, treeName, histDirectoryName, x=None, y=None, nBins_x=None, nBins_y=None):
    position_relative_x_bins, position_relative_y_bins = get_photosensor_hits_position_relative_bins(fileName, treeName, histDirectoryName, x, y, nBins_x, nBins_y)

    return list(zip(position_relative_x_bins.cat.codes, position_relative_y_bins.cat.codes))

def get_photosensor_hits_position_initial(fileName, treeName='photoSensor_hits;1'):
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

def get_photosensor_hits_photosensor_ID(fileName, treeName='photoSensor_hits;1', useHistograms=False, histDirectory='/photoSensor_hits_histograms', verbose=True):
    file = uproot.open(fileName)

    if not useHistograms:
        try:
            tree = file[treeName]
            photosensor_id = tree['photoSensor_hits_photoSensorID'].array()
        except:
            useHistograms = True
            if verbose:
                print('No tree found, using histograms instead with `histDirectory` =', histDirectory)

    if useHistograms:
        photosensor_id = get_histogram_titles(fileName, histDirectory)

    file.close()
    return photosensor_id

def get_photosensor_hits_photosensor_position(fileName, treeName='photoSensor_hits;1', useHistograms=False, histDirectory='/photoSensor_hits_histograms', verbose=True):
    IDs = get_photosensor_hits_photosensor_ID(fileName, treeName, useHistograms, histDirectory, verbose)
    positions = [(float(ID.split('_')[2]), float(ID.split('_')[3]), float(ID.split('_')[4])) for ID in IDs]
    return positions

def get_photosensor_hits_photosensor_wall(fileName, treeName='photoSensor_hits;1', useHistograms=False, histDirectory='/photoSensor_hits_histograms', verbose=True):
    IDs = get_photosensor_hits_photosensor_ID(fileName, treeName, useHistograms, histDirectory, verbose)
    walls = [ID.split('_')[1] for ID in IDs]
    return walls

def get_photosensor_hits_photosensor_direction(fileName, treeName='photoSensor_hits;1', useHistograms=False, histDirectory='/photoSensor_hits_histograms', verbose=True):
    walls = get_photosensor_hits_photosensor_wall(fileName, treeName, useHistograms, histDirectory, verbose)

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

def get_primary_pdg(fileName, treeName):
    file = uproot.open(fileName)
    tree = file[treeName]
    pdg = tree['primary_pdg'].array()
    file.close()
    return pdg

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

def fixRotations_relative_signs(positions, walls, handleZ=True):
    indices = np.where(walls == '+y')
    sign = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN['+y']
    if not handleZ:
        sign = (sign[0], sign[1], 1)
    positions[indices] = positions[indices] * sign

    indices = np.where(walls == '-x')
    sign = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN['-x']
    if not handleZ:
        sign = (sign[0], sign[1], 1)
    positions[indices] = positions[indices] * sign

    indices = np.where(walls == '-z')
    sign = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN['-z']
    if not handleZ:
        sign = (sign[0], sign[1], 1)
    positions[indices] = positions[indices] * sign

    indices = np.where(walls == '+x')
    sign = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN['+x']
    if not handleZ:
        sign = (sign[0], sign[1], 1)
    positions[indices] = positions[indices] * sign

    indices = np.where(walls == '+z')
    sign = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN['+z']
    if not handleZ:
        sign = (sign[0], sign[1], 1)
    positions[indices] = positions[indices] * sign

    indices = np.where(walls == '-y')
    sign = RELATIVE_SENSOR_POSITIONS_XYZ_SIGN['-y']
    if not handleZ:
        sign = (sign[0], sign[1], 1)
    positions[indices] = positions[indices] * sign

    return positions.tolist()