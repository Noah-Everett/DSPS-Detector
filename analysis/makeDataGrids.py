import argparse
import os
import sys
import uproot
import numpy as np
import pandas as pd

sys.path.append('../python/')
from gridMethods import *
from importMethods import *
# get_histogram_hits_tuple, get_histogram_sizes, get_histogram_nHits_total

def checkFiles(paths_input, shape):
    for path in paths_input:
        try:
            f = uproot.open(path)
        except:
            print(f"Error: could not open file {path}. Skipping file.")
            paths_input.remove(path)
            continue

        try:
            histsDir = f['photoSensor_hits_histograms']
        except:
            print(f"Error: could not find photoSensor_hits_histograms dir in file {path}. Skipping file.")
            paths_input.remove(path)
            continue

        try:
            for key in histsDir.keys():
                h = histsDir[key]
                if h.allnumpy().shape != shape:
                    print(f"Error: histogram {key} in file {path} has shape {h.allnumpy().shape}, not {shape}. Skipping file.")
                    paths_input.remove(path)
                    break
        except:
            print(f"Error: could not read histogram in file {path}. Skipping file.")
            paths_input.remove(path)
            continue

    return paths_input

def applyCuts(paths_input, minNHits, minNPrimarySteps):
    filtered_paths = []
    for path in paths_input:
        try:
            nHits = get_histogram_nHits_total(path, directoryName='photoSensor_hits_histograms')
            sizes = get_histogram_sizes(path, directoryName='photoSensor_hits_histograms')

            if nHits >= minNHits and len(sizes) >= minNPrimarySteps:
                filtered_paths.append(path)
        except Exception as e:
            print(f"Error processing file {path}: {e}")

    return filtered_paths

def processFiles(paths_input, output_dir, grid_size):
    for path in paths_input:
        try:
            data = get_histogram_hits_tuple(path, directoryName='photoSensor_hits_histograms')
            output_path = os.path.join(output_dir, os.path.basename(path).replace('.root', '.npy'))
            np.save(output_path, data)
        except Exception as e:
            print(f"Error processing file {path}: {e}")

def main(args):
    paths_input = [os.path.join(args.dir_input, f) for f in os.listdir(args.dir_input) if f.endswith('.root')]

    if args.checkFiles:
        paths_input = checkFiles(paths_input, args.gridSize)
    if args.doCuts:
        paths_input = applyCuts(paths_input, args.minNHits, args.minNPrimarySteps)

    processFiles(paths_input, args.dir_output, args.gridSize)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Train UNet')

    # add required argument for input data directory
    parser.add_argument('dir_input', type=str, help='Path to input data directory. Note: all ROOT files in this directory will be used as input data')

    # add required argument for output data directory
    parser.add_argument('dir_output', type=str, help='Path to generated output data directory')

    # add optional argument for grid size (default is 80,80,80)
    parser.add_argument('--gridSize', type=int, nargs=3, default=[80,80,80], help='Grid size (default: 80,80,80)')

    # add optional argument for minNHits (default is 0)
    parser.add_argument('--minNHits', type=int, default=0, help='Minimum number of hits (default: 0)')

    # add optional argument for minNPrimarySteps (default is 30)
    parser.add_argument('--minNPrimarySteps', type=int, default=30, help='Minimum number of primary steps (default: 30)')

    # add optional argument for doCuts (default is True)
    parser.add_argument('--doCuts', type=bool, default=True, help='Apply cuts to data (default: True)')

    # add optional argument for checkFiles (default is True)
    parser.add_argument('--checkFiles', type=bool, default=True, help='Check if run ROOT files (1) exist, (2) all DSPS hit histograms are the same size (default: True)')

    args = parser.parse_args()

    main(args)