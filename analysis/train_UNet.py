###################
##### IMPORTS #####
###################
import sys
import numpy as np
import os
import torch

from UNetMethods import *

sys.path.append('pytorch-3dunet/')
from pytorch3dunet.train import main

####################
##### SETTINGS #####
####################
useHistograms = True # if True use histograms, if False use tupple data

checkFiles = False # check if run ROOT files (1) exit, (2) have enough hits, (3) have enough primary particle steps, (4) all DSPS hit histograms are the same size

dataDir = ''
tmpDir  = dataDir+''

fileNumbers = [i for i in range(35)] # the ML data set has 1138 files
runDataRoot_filePaths = ['/Users/noah-everett/Documents/FNAL/Geant4/USSD_Geant4/runs/MLtrainData/multievent_{}.root'.format(i) for i in fileNumbers]
runDataRoot_hits_histDir = '/photoSensor_hits_histograms'
runDataRoot_hits_treeName = 'photoSensor_hits;1'
runDataRoot_primary_treeName = 'primary;1'

saveDf_hits                = True
saveDf_primary             = True
saveDfOnlyIfNoFile_hits    = True
saveDfOnlyIfNoFile_primary = True
saveDf_hits_basePath       = tmpDir+'simDF/MLdata_DF_hits'    # directory and beginning of file name
saveDf_primary_basePath    = tmpDir+'simDF/MLdata_DF_primary' # directory and beginning of file name

saveGrid_hits_npy                = False
saveGrid_primary_npy             = False
saveGrids_h5                     = True
saveGridOnlyIfNoFile_hits_npy    = True
saveGridOnlyIfNoFile_primary_npy = True
saveGridOnlyIfNoFile_h5          = True
saveGrid_hits_basePath_npy       = tmpDir+'MLdata/MLdata_grid_hits'    # directory and beginning of file name
saveGrid_primary_basePath_npy    = tmpDir+'MLdata/MLdata_grid_primary' # directory and beginning of file name
saveGrids_h5                     = tmpDir+'MLdata/MLdata_grids'        # directory and beginning of file name

saveModel          = True
saveModel_basePath = 'data/MLmodels/MLmodel' # directory and beginning of file name

#################################
##### MAKE INPUT PATH LISTS #####
#################################
dfPaths_hits    = [f"{saveDf_hits_basePath}_{i}.parquet"    for i in fileNumbers]
dfPaths_primary = [f"{saveDf_primary_basePath}_{i}.parquet" for i in fileNumbers]

#########################
##### MORE SETTINGS #####
#########################
N = 80
gridSize_input  = (N, N, N)
gridSize_output = (N, N, N)

voxelGrid_make_errors               = False
voxelGrid_make_walls                = True
voxelGrid_make_walls_combine        = False

voxelGrid_make_vectors_combine      = True

###########################
##### MAKE PATH LISTS #####
###########################
gridPaths_hits_npy    = [f"{saveGrid_hits_basePath_npy}_{i}_{gridSize_input[0]}x{gridSize_input[1]}x{gridSize_input[2]}_{'withWalls' if voxelGrid_make_walls else 'noWalls'}_{'withErrors' if voxelGrid_make_errors else 'noErrors'}.npy" for i in fileNumbers]
gridPaths_primary_npy = [f"{saveGrid_primary_basePath_npy}_{i}_{gridSize_output[0]}x{gridSize_output[1]}x{gridSize_output[2]}.npy"                                                                                                        for i in fileNumbers]
gridPaths_h5          = [f"{saveGrids_h5}_{i}_{gridSize_output[0]}x{gridSize_output[1]}x{gridSize_output[2]}_{'withWalls' if voxelGrid_make_walls else 'noWalls'}_{'withErrors' if voxelGrid_make_errors else 'noErrors'}.h5"             for i in fileNumbers]

print('gridPaths_hits_npy[0]:', gridPaths_hits_npy[0])
print('gridPaths_primary_npy[0]:', gridPaths_primary_npy[0])
print('gridPaths_h5[0]:', gridPaths_h5[0])

#####################
##### LOAD DATA #####
#####################
nTest = 20
nVal  = 10

np.random.seed(42)

testIndices = np.random.choice(len(dfPaths_hits), nTest, replace=False)
valIndices  = np.random.choice(np.delete(np.arange(len(dfPaths_hits)), testIndices), nVal, replace=False)

X_train_indices = np.delete(np.arange(len(dfPaths_hits)), np.concatenate((testIndices, valIndices)))
Y_train_indices = np.delete(np.arange(len(dfPaths_hits)), np.concatenate((testIndices, valIndices)))
X_test_indices  = np.array(testIndices)
Y_test_indices  = np.array(testIndices)
X_val_indices   = np.array(valIndices)
Y_val_indices   = np.array(valIndices)

paths_train = [gridPaths_h5[i] for i in X_train_indices]
paths_test  = [gridPaths_h5[i] for i in X_test_indices]
paths_val   = [gridPaths_h5[i] for i in X_val_indices]

# print('X_train_indices:', X_train_indices.tolist())
# print('X_test_indices:' , X_test_indices .tolist())
# print('X_val_indices:'  , X_val_indices  .tolist())

print('len(paths_train):', len(paths_train))
print('len(paths_test):' , len(paths_test))
print('len(paths_val):'  , len(paths_val))

assert len(paths_train) == len(X_train_indices)
assert len(paths_test ) == len(X_test_indices )
assert len(paths_val  ) == len(X_val_indices  )

assert len(np.intersect1d(X_train_indices, X_test_indices)) == 0, f"Overlap: {np.intersect1d(X_train_indices, X_test_indices)}"
assert len(np.intersect1d(X_train_indices, X_val_indices )) == 0, f"Overlap: {np.intersect1d(X_train_indices, X_val_indices )}"
assert len(np.intersect1d(X_test_indices , X_val_indices )) == 0, f"Overlap: {np.intersect1d(X_test_indices , X_val_indices )}"

#################
##### TRAIN #####
#################
config = get_config(paths_train, paths_val, 'checkpoints', num_workers=10, device='cuda')
# config = get_config(tmpDir+'/train/', tmpDir+'/val/', num_workers=10)
save_config(config, 'config.yml')

main(['--config', 'config.yml'])