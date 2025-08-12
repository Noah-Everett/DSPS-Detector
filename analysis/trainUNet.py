#!/usr/bin/env python3
"""
Train a 3D U-Net model using PyTorch.

This script prepares the dataset by identifying `.h5` files in the specified data directory,
splits them into training, validation, and test sets, configures the training parameters,
and initiates the training process.

Usage Examples:
    1. Auto-detecting min and max file numbers:
        python train_model.py --data-dir /path/to/data \
                              --output-dir /path/to/output

    2. Specifying only the maximum file number (auto-detects min-number):
        python train_model.py --data-dir /path/to/data \
                              --output-dir /path/to/output \
                              --max-number 50

    3. Specifying both min and max file numbers:
        python train_model.py --data-dir /path/to/data \
                              --output-dir /path/to/output \
                              --min-number 10 \
                              --max-number 50

    4. Customizing training parameters:
        python train_model.py --data-dir /path/to/data \
                              --output-dir /path/to/output \
                              --min-number 10 \
                              --max-number 50 \
                              --num-workers 8 \
                              --device cpu \
                              --batch-size 2 \
                              --verbosity debug
"""

import argparse
import logging
import os
import sys
import re
import numpy as np

sys.path.append('../pytorch-3dunet/')
from pytorch3dunet.train import main as train_main

sys.path.append('../python/')
from UNetMethods import get_config_train, save_config

def configure_logging(verbosity: str):
    """
    Configure root logger formatting and level.
    """
    level = getattr(logging, verbosity.upper(), None)
    if not isinstance(level, int):
        raise ValueError(f"Invalid verbosity level: {verbosity}")
    logging.basicConfig(
        level=level,
        format='%(asctime)s - %(name)s - %(filename)s::%(funcName)s(%(lineno)d) - %(levelname)s - %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S'
    )
    # Set logger verbosity and prevent duplicate logging
    loggers_to_configure = ["UNetTrainer", "HDF5Dataset", "Dataset"]
    for logger_name in loggers_to_configure:
        logger = logging.getLogger(logger_name)
        logger.setLevel(level)
        logger.propagate = False

def parse_arguments():
    parser = argparse.ArgumentParser(description="Train a 3D U-Net model.")

    # Data and output directories
    parser.add_argument('--data-dir', type=str, required=True,
                        help='Directory containing the .h5 dataset files.')
    parser.add_argument('--output-dir', type=str, required=True,
                        help='Directory to save outputs like models and configs.')

    # File number specifications
    parser.add_argument('--min-number', type=int, default=-1,
                        help='Minimum file number (inclusive). Default is -1 (auto-detect).')
    parser.add_argument('--max-number', type=int, default=-1,
                        help='Maximum file number (inclusive). Default is -1 (auto-detect).')

    # Training parameters
    parser.add_argument('--num-workers', type=int, default=1,
                        help='Number of worker threads for data loading.')
    parser.add_argument('--device', type=str, default='cuda',
                        help='Device to use for training (e.g., "cuda" or "cpu").')
    parser.add_argument('--batch-size', type=int, default=1,
                        help='Batch size for training.')
    parser.add_argument('--resume-path', type=str, default=None,
                        help='Path to a checkpoint file to resume training from.')

    # Logging parameters
    parser.add_argument('--verbosity', '-v', type=str, default='info',
                        choices=['debug', 'info', 'warning', 'error', 'critical'],
                        help='Set the logging verbosity level.')

    return parser.parse_args()

def discover_file_numbers(data_dir):
    """
    Discover all file numbers in the data directory based on the expected file naming pattern.
    Expected pattern: {number}.h5 (e.g., 0.h5, 1.h5, ...)
    """
    if not os.path.isdir(data_dir):
        raise FileNotFoundError(f"Data directory not found: {data_dir}")

    pattern = re.compile(r'^(\d+)\.h5$')
    file_numbers = []

    for filename in os.listdir(data_dir):
        match = pattern.match(filename)
        if match:
            file_num = int(match.group(1))
            file_numbers.append(file_num)

    if not file_numbers:
        raise ValueError(f"No .h5 files found in {data_dir} matching the expected pattern.")

    return sorted(file_numbers)

def prepare_file_numbers(data_dir, min_number, max_number):
    """
    Generate a list of file numbers based on min_number and max_number.
    If min_number or max_number is -1, auto-detect from the data directory.
    """
    available_numbers = discover_file_numbers(data_dir)
    auto_min = min(available_numbers)
    auto_max = max(available_numbers)

    # Determine actual min and max
    actual_min = min_number if min_number != -1 else auto_min
    actual_max = max_number if max_number != -1 else auto_max

    if actual_min > actual_max:
        raise ValueError(f"min-number ({actual_min}) cannot be greater than max-number ({actual_max}).")

    # Filter available_numbers within the specified range
    filtered_numbers = [num for num in available_numbers if actual_min <= num <= actual_max]

    if not filtered_numbers:
        raise ValueError(f"No files found in the range {actual_min} to {actual_max}.")

    return filtered_numbers

def prepare_file_paths(data_dir, file_numbers):
    """
    Prepare the list of grid file paths based on the provided file numbers.
    Assumes files are named as {number}.h5 (e.g., 0.h5, 1.h5).
    """
    grid_paths = [
        os.path.join(data_dir, f"{i}.h5")
        for i in file_numbers
    ]
    return grid_paths

def split_dataset(file_paths, n_test=20, n_val=10, seed=42):
    """
    Split the dataset into training, validation, and test sets.
    """
    np.random.seed(seed)
    total = len(file_paths)
    if total < (n_test + n_val):
        raise ValueError(f"Not enough files to split into test and validation sets. Required: {n_test + n_val}, Available: {total}")

    indices = np.arange(total)
    test_indices = np.random.choice(total, n_test, replace=False)
    remaining = np.setdiff1d(indices, test_indices)
    val_indices = np.random.choice(remaining, n_val, replace=False)
    train_indices = np.setdiff1d(remaining, val_indices)

    paths_train = [file_paths[i] for i in train_indices]
    paths_val = [file_paths[i] for i in val_indices]
    paths_test = [file_paths[i] for i in test_indices]

    return paths_train, paths_val, paths_test

def check_file_existence(file_paths):
    """
    Check if all files in file_paths exist.
    Returns a list of missing files.
    """
    missing = [path for path in file_paths if not os.path.isfile(path)]
    return missing

def main():
    args = parse_arguments()
    
    # Configure logging
    configure_logging(args.verbosity)
    logger = logging.getLogger(__name__)

    # Create output directories if they don't exist
    os.makedirs(args.output_dir, exist_ok=True)
    checkpoints_dir = os.path.join(args.output_dir, 'checkpoints')
    os.makedirs(checkpoints_dir, exist_ok=True)
    config_path = os.path.join(args.output_dir, 'config.yml')

    # Prepare file numbers
    try:
        file_numbers = prepare_file_numbers(args.data_dir, args.min_number, args.max_number)
    except (FileNotFoundError, ValueError) as e:
        logger.error(f"Error in preparing file numbers: {e}")
        return

    logger.info(f"Selected file numbers: {file_numbers}")

    # Prepare file paths
    grid_paths = prepare_file_paths(args.data_dir, file_numbers)
    logger.info(f"Total grid files found: {len(grid_paths)}")

    # Verify that all grid files exist
    missing_files = check_file_existence(grid_paths)
    if missing_files:
        logger.warning("The following grid files are missing:")
        for path in missing_files:
            logger.warning(f" - {path}")
        logger.warning("Please ensure all specified grid files exist.")
        # Exit to prevent training with missing data
        return

    # Split dataset
    try:
        paths_train, paths_val, paths_test = split_dataset(grid_paths)
    except ValueError as e:
        logger.error(f"Error in dataset splitting: {e}")
        return

    logger.info(f"Training samples: {len(paths_train)}")
    logger.info(f"Validation samples: {len(paths_val)}")
    logger.info(f"Test samples: {len(paths_test)}")

    # Configure training
    config = get_config_train(
        paths_train=paths_train,
        paths_val=paths_val,
        checkpoint_dir=checkpoints_dir,
        num_workers=args.num_workers,
        device=args.device,
        batchSize=args.batch_size,
        resume=args.resume_path,
    )

    # Save configuration
    save_config(config, config_path)
    logger.info(f"Configuration saved to {config_path}")

    import torch
    torch.backends.cudnn.benchmark = True             # better conv autotuning (variable image sizes ok)
    torch.backends.cuda.matmul.allow_tf32 = True      # Ampere/Hopper: big win for matmuls
    torch.set_float32_matmul_precision("high")        # PyTorch 2.x: enable faster kernels


    # Start training
    logger.info("Starting training...")
    train_main(['--config', config_path])

if __name__ == '__main__':
    main()