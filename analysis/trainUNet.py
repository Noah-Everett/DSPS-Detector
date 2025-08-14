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

    5. Overriding with one or more user YAMLs (later wins on conflicts):
        # multiple flags
        python train_model.py --data-dir /path/to/data \
                              --output-dir /path/to/output \
                              --config base.yml --config exp1.yml

        # or comma-separated
        python train_model.py --data-dir /path/to/data \
                              --output-dir /path/to/output \
                              --config base.yml,exp1.yml,local.yml
"""

import argparse
import logging
import os
import sys
import re
import copy
import numpy as np
import yaml

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'pytorch-3dunet')))
from pytorch3dunet.train import main as train_main

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'python')))
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
    parser.add_argument('--iters-val', type=int, default=-1,
                        help='Number of iterations for validation.')
    parser.add_argument('--iters-max', type=int, default=-1,
                        help='Maximum number of iterations.')
    parser.add_argument('--iters-log', type=int, default=-1,
                        help='Number of iterations between logging.')
    parser.add_argument('--epochs-max', type=int, default=1000,
                        help='Maximum number of epochs.')
    parser.add_argument('--learning-rate', '-lr', type=float, default=0.0002,
                        help='Learning rate for the optimizer.')

    # Logging parameters
    parser.add_argument('--verbosity', '-v', type=str, default='info',
                        choices=['debug', 'info', 'warning', 'error', 'critical'],
                        help='Set the logging verbosity level.')

    # One or more user-provided YAMLs that override generated config.
    # Accepts repeated flags (--config a.yml --config b.yml) or comma-separated (--config a.yml,b.yml).
    parser.add_argument('--config', action='append', default=None,
                        help='Path(s) to YAML file(s) whose values overwrite the generated config. '
                             'Repeat the flag or use comma-separated values. Later files take precedence.')

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

def load_yaml_file(path: str):
    with open(path, 'r') as f:
        return yaml.safe_load(f) or {}

def deep_merge(base: dict, override: dict, path_prefix=""):
    """
    Recursively merge `override` into `base` (in place), replacing scalars/lists and
    recursing into dicts. Returns a list of dotted paths that changed.
    """
    changes = []

    for k, v in override.items():
        dotted = f"{path_prefix}.{k}" if path_prefix else k
        if isinstance(v, dict) and isinstance(base.get(k), dict):
            changes.extend(deep_merge(base[k], v, dotted))
        else:
            before = base.get(k, "<MISSING>")
            base[k] = copy.deepcopy(v)
            changes.append((dotted, before, v))
    return changes

def log_changes(logger, changes, source_label=None):
    if not changes:
        logger.info(f"No overrides applied from YAML{f' [{source_label}]' if source_label else ''} (keys matched nothing or YAML was empty).")
        return
    header = f"Applied YAML overrides{f' from {source_label}' if source_label else ''}:"
    logger.info(header)
    for key, before, after in changes:
        logger.info(f"  - {key}: {before} -> {after}")

def _normalize_config_args(config_args):
    """
    Normalize argparse --config input to a flat, ordered list of paths.
    Accepts repeated flags and/or comma-separated values.
    """
    if not config_args:
        return []
    paths = []
    for entry in config_args:
        # argparse with action='append' gives us each flag occurrence as a string
        parts = [p.strip() for p in str(entry).split(',') if p.strip()]
        paths.extend(parts)
    # keep order; no dedup to preserve intended layering
    return paths

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

    # Adjust iteration parameters if not set
    # Set default values based on training set size
    # Because each file is one iteration
    if args.iters_val < 0:
        args.iters_val = len(paths_train)
    if args.iters_log < 0:
        args.iters_log = len(paths_train)
    if args.iters_max < 0:
        args.iters_max = len(paths_train) * args.epochs_max

    # Configure training (generated defaults)
    config = get_config_train(
        paths_train=paths_train,
        paths_val=paths_val,
        checkpoint_dir=checkpoints_dir,
        num_workers=args.num_workers,
        device=args.device,
        batchSize=args.batch_size,
        resume=args.resume_path,
        iters_val=args.iters_val,
        iters_max=args.iters_max,
        iters_log=args.iters_log,
        epochs_max=args.epochs_max,
        lr=args.learning_rate,
    )

    # Merge user YAML(s) over the generated config
    config_paths = _normalize_config_args(args.config)
    if config_paths:
        logger.info(f"Applying {len(config_paths)} config override file(s) in order (later wins): {config_paths}")
        for cfg_idx, cfg_path in enumerate(config_paths, start=1):
            user_cfg_path = os.path.abspath(cfg_path)
            if not os.path.isfile(user_cfg_path):
                logger.error(f"--config file not found: {user_cfg_path}")
                return
            try:
                user_cfg = load_yaml_file(user_cfg_path)
            except Exception as e:
                logger.error(f"Failed to parse YAML at {user_cfg_path}: {e}")
                return

            if not isinstance(user_cfg, dict):
                logger.error(f"Top-level YAML must be a mapping/dict in {user_cfg_path}, got {type(user_cfg).__name__}.")
                return

            changes = deep_merge(config, user_cfg)
            label = f"{os.path.basename(user_cfg_path)} (#{cfg_idx})"
            log_changes(logger, changes, source_label=label)

    # Save configuration (merged if YAMLs provided)
    save_config(config, config_path)
    logger.info(f"Configuration saved to {config_path}")

    import torch
    torch.backends.cudnn.benchmark = True
    torch.backends.cuda.matmul.allow_tf32 = True
    torch.backends.cudnn.allow_tf32 = True
    torch.set_float32_matmul_precision("high")

    # Start training
    logger.info("Starting training...")
    try:
        # pytorch3dunet expects CLI-like argv
        old_args = sys.argv.copy()
        sys.argv = ['train.py', '--config', config_path]
        train_main()
        sys.argv = old_args
    except SystemExit as e:
        # Some entrypoints call sys.exit(); surface as info if zero else error
        code = int(getattr(e, 'code', 1))
        if code == 0:
            logger.info("Training completed successfully.")
        else:
            logger.error(f"Training failed with exit code {code}.")
            return

if __name__ == '__main__':
    main()