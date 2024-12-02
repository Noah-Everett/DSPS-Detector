import argparse
import os
import uproot

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
    for path in paths_input:
        f = uproot.open(path)
        histsDir = f['photoSensor_hits_histograms']

def main(args):
    paths_input = [f"{args.dir_input}/{f}" for f in os.listdir(args.dir_input) if f.endswith('.root')]

    if args.checkFiles:
        paths_input = checkFiles(paths_input, args.gridSize)
    if args.doCuts:

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