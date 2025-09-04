#!/usr/bin/env python3
"""
Run predictions with a 3D U-Net model (PyTorch 3D U-Net).

This script:
  • Collects .h5 volumes from a data directory (by numeric filename like 0.h5, 1.h5, ...),
  • Builds a prediction config using your UNetMethods.get_config_predict,
  • Optionally applies one or more user YAMLs that override the generated config (later wins),
  • Saves the (possibly merged) config to the output directory,
  • Invokes pytorch3dunet.predict.main with that config.

Examples
--------
1) Auto-detect all file numbers found in the directory:
    python predUNet.py --data-dir /path/to/data \
                       --output-dir /path/to/output \
                       --model-path /path/to/checkpoints/last_checkpoint.pytorch

2) Specify a closed range:
    python predUNet.py --data-dir /path/to/data \
                       --output-dir /path/to/output \
                       --model-path /path/to/checkpoints/last_checkpoint.pytorch \
                       --min-number 200 --max-number 220

3) Specify an explicit list (overrides min/max if provided):
    python predUNet.py --data-dir /path/to/data \
                       --output-dir /path/to/output \
                       --model-path /path/to/checkpoints/last_checkpoint.pytorch \
                       --numbers 0,1,2,217,900

4) Just write the config file (no prediction run), useful for inspection:
    python predUNet.py --data-dir /path/to/data \
                       --output-dir /path/to/output \
                       --model-path /path/to/checkpoints/last_checkpoint.pytorch \
                       --config-only

5) Override with one or more user YAMLs (later wins on conflicts):
    # multiple flags
    python predUNet.py --data-dir /path/to/data \
                       --output-dir /path/to/output \
                       --model-path /path/to/checkpoints/last_checkpoint.pytorch \
                       --config base.yml --config exp1.yml

    # or comma-separated
    python predUNet.py --data-dir /path/to/data \
                       --output-dir /path/to/output \
                       --model-path /path/to/checkpoints/last_checkpoint.pytorch \
                       --config base.yml,exp1.yml,local.yml

Notes
-----
• Files must be named as {number}.h5 (e.g., 0.h5, 1.h5, ...).
• Missing files are warned about and skipped.
• The generated/merged config is saved as <output-dir>/config.yml.
"""

import argparse
import logging
import os
import re
import sys
import copy
import yaml
from typing import List, Sequence

# --- Local imports ------------------------------------------------------------
# Keep your original relative paths so we reuse your helpers.
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'python'))
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'pytorch-3dunet'))

try:
    from pytorch3dunet.predict import main as predict_main
except Exception as e:
    # Defer raising until after logging is configured in main()
    _IMPORT_ERROR = e  # noqa: N816
else:
    _IMPORT_ERROR = None

from UNetMethods import get_config_predict, save_config  # type: ignore


# --- Logging -----------------------------------------------------------------
def configure_logging(verbosity: str) -> None:
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
    # Quiet very chatty third-party loggers unless we're debugging
    if level > logging.DEBUG:
        for noisy in ("matplotlib", "h5py", "urllib3"):
            logging.getLogger(noisy).setLevel(logging.WARNING)


# --- YAML helpers (mirrors train script behavior) -----------------------------
def load_yaml_file(path: str):
    with open(path, 'r') as f:
        return yaml.safe_load(f) or {}

def deep_merge(base: dict, override: dict, path_prefix: str = ""):
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

def log_changes(logger: logging.Logger, changes, source_label: str | None = None):
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
        parts = [p.strip() for p in str(entry).split(',') if p.strip()]
        paths.extend(parts)
    # keep order; no dedup to preserve intended layering
    return paths


# --- Argument parsing ---------------------------------------------------------
def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Predict with a 3D U-Net model.")
    # Required I/O
    parser.add_argument('--data-dir', type=str, required=True,
                        help='Directory containing .h5 volumes named as {number}.h5.')
    parser.add_argument('--output-dir', type=str, required=True,
                        help='Directory to save outputs and the generated config.')
    parser.add_argument('--model-path', type=str, required=True,
                        help='Path to a .pytorch checkpoint to use for inference.')

    # File selection
    parser.add_argument('--min-number', type=int, default=-1,
                        help='Minimum file number (inclusive). Default: auto-detect.')
    parser.add_argument('--max-number', type=int, default=-1,
                        help='Maximum file number (inclusive). Default: auto-detect.')
    parser.add_argument('--numbers', type=str, default=None,
                        help='Comma-separated explicit list of file numbers (e.g. "0,1,2,217"). '
                             'This overrides --min-number/--max-number if provided.')

    # Misc
    parser.add_argument('--config-name', type=str, default='config.yml',
                        help='Name of the config file to write in output-dir. Default: config.yml')
    parser.add_argument('--config-only', action='store_true',
                        help='Write the config and exit without running prediction.')
    parser.add_argument('--verbosity', '-v', type=str, default='info',
                        choices=['debug', 'info', 'warning', 'error', 'critical'],
                        help='Logging verbosity.')
    parser.add_argument('--device', type=str, default='cuda',
                        choices=['cuda', 'cpu'],
                        help='Device to use for inference.')

    # Accepts repeated flags (--config a.yml --config b.yml) or comma-separated (--config a.yml,b.yml).
    parser.add_argument('--config', action='append', default=None,
                        help='Path(s) to YAML file(s) whose values overwrite the generated config. '
                             'Repeat the flag or use comma-separated values. Later files take precedence.')
    parser.add_argument('--model-config', type=str, default=None,
                        help='Path to the model config file.')

    return parser.parse_args()


# --- Utilities ----------------------------------------------------------------
_NUM_PATTERN = re.compile(r'^(\d+)\.h5$')


def discover_file_numbers(data_dir: str) -> List[int]:
    """
    Discover numeric file IDs based on {number}.h5 files in data_dir.
    """
    if not os.path.isdir(data_dir):
        raise FileNotFoundError(f"Data directory not found: {data_dir}")

    numbers: List[int] = []
    for name in os.listdir(data_dir):
        m = _NUM_PATTERN.match(name)
        if m:
            numbers.append(int(m.group(1)))

    if not numbers:
        raise ValueError(f"No .h5 files found in {data_dir} with pattern '{{number}}.h5'.")

    return sorted(numbers)


def parse_numbers_arg(numbers_arg: str) -> List[int]:
    """
    Parse a comma-separated list of integers into a sorted unique list.
    """
    raw = [s.strip() for s in numbers_arg.split(',') if s.strip() != '']
    try:
        nums = sorted(set(int(s) for s in raw))
    except ValueError as e:
        raise ValueError(f"Invalid --numbers list: {numbers_arg}") from e
    return nums


def select_numbers(data_dir: str,
                   min_number: int,
                   max_number: int,
                   explicit_numbers: Sequence[int] | None) -> List[int]:
    """
    Choose file numbers to predict on:
      • If explicit_numbers is provided (non-empty), use that.
      • Else, discover from the directory and apply min/max if given.
    """
    if explicit_numbers:
        return sorted(set(explicit_numbers))

    available = discover_file_numbers(data_dir)
    auto_min, auto_max = min(available), max(available)
    actual_min = auto_min if min_number == -1 else min_number
    actual_max = auto_max if max_number == -1 else max_number

    if actual_min > actual_max:
        raise ValueError(f"min-number ({actual_min}) cannot be greater than max-number ({actual_max}).")

    return [n for n in available if actual_min <= n <= actual_max]


def build_paths(data_dir: str, numbers: Sequence[int]) -> List[str]:
    """
    Build absolute paths for {number}.h5 and drop any that don't exist.
    """
    paths = [os.path.join(data_dir, f"{n}.h5") for n in numbers]
    missing = [p for p in paths if not os.path.isfile(p)]
    if missing:
        logger = logging.getLogger(__name__)
        for m in missing:
            logger.warning("Missing file skipped: %s", m)
        paths = [p for p in paths if os.path.isfile(p)]
    return paths


# --- Main ---------------------------------------------------------------------
def main() -> None:
    args = parse_arguments()
    configure_logging(args.verbosity)
    logger = logging.getLogger("predUNet")

    # Early import error surface (after logging config)
    if _IMPORT_ERROR is not None:
        logger.error("Failed to import pytorch3dunet.predict.main: %s", _IMPORT_ERROR)
        logger.error("Ensure '../pytorch-3dunet/' is correct and dependencies are installed.")
        raise _IMPORT_ERROR

    # Ensure I/O dirs
    os.makedirs(args.output_dir, exist_ok=True)
    config_path = os.path.join(args.output_dir, args.config_name)

    # File selection
    try:
        explicit_numbers = parse_numbers_arg(args.numbers) if args.numbers else None
        numbers = select_numbers(args.data_dir, args.min_number, args.max_number, explicit_numbers)
    except (FileNotFoundError, ValueError) as e:
        logger.error("File selection error: %s", e)
        sys.exit(1)

    if not numbers:
        logger.error("No file numbers selected.")
        sys.exit(1)

    logger.info("Selected %d file(s). First few: %s", len(numbers), numbers[:10])

    # Build and check paths
    data_paths = build_paths(args.data_dir, numbers)
    if not data_paths:
        logger.error("No existing .h5 paths after filtering. Nothing to predict.")
        sys.exit(1)

    logger.info("Total existing .h5 paths: %d", len(data_paths))

    # Model path sanity check
    if not os.path.isfile(args.model_path):
        logger.error("Model checkpoint not found: %s", args.model_path)
        sys.exit(1)

    # Build config via your helper
    try:
        config = get_config_predict(data_paths, args.model_path, args.output_dir, device=args.device)
    except Exception as e:
        logger.error("Error constructing prediction config: %s", e)
        sys.exit(1)

    # Merge model config
    if args.model_config:
        if not os.path.isfile(args.model_config):
            logger.error("Model config file not found: %s", args.model_config)
            sys.exit(1)
        try:
            model_config = load_yaml_file(args.model_config)
            if not isinstance(model_config, dict):
                raise ValueError(f"Model config must be a mapping/dict, got {type(model_config).__name__}.")
            changes = deep_merge(config['model'], model_config['model'], path_prefix='model')
            log_changes(logger, changes)
        except Exception as e:
            logger.error("Failed to load or parse model config: %s", e)
            sys.exit(1)

    # Merge user YAML(s) over the generated config (later files win)
    config_paths = _normalize_config_args(args.config)
    if config_paths:
        logger.info(f"Applying {len(config_paths)} config override file(s) in order (later wins): {config_paths}")
        for cfg_idx, cfg_path in enumerate(config_paths, start=1):
            user_cfg_path = os.path.abspath(cfg_path)
            if not os.path.isfile(user_cfg_path):
                logger.error(f"--config file not found: {user_cfg_path}")
                sys.exit(1)
            try:
                user_cfg = load_yaml_file(user_cfg_path)
            except Exception as e:
                logger.error(f"Failed to parse YAML at {user_cfg_path}: {e}")
                sys.exit(1)

            if not isinstance(user_cfg, dict):
                logger.error(f"Top-level YAML must be a mapping/dict in {user_cfg_path}, got {type(user_cfg).__name__}.")
                sys.exit(1)

            changes = deep_merge(config, user_cfg)
            label = f"{os.path.basename(user_cfg_path)} (#{cfg_idx})"
            log_changes(logger, changes, source_label=label)

    # Save config (merged if YAMLs provided)
    try:
        save_config(config, config_path)
        logger.info("Config written to %s", config_path)
    except Exception as e:
        logger.error("Failed to write config: %s", e)
        sys.exit(1)

    # Optionally stop here
    if args.config_only:
        logger.info("--config-only set; exiting before running prediction.")
        return

    # Run prediction
    logger.info("Starting prediction with the following config: %s", config)
    try:
        # pytorch3dunet expects CLI-like argv
        old_args = sys.argv.copy()
        sys.argv = ['predict.py', '--config', config_path]
        predict_main()
        sys.argv = old_args
    except SystemExit as e:
        # Some entrypoints call sys.exit(); surface as info if zero else error
        code = int(getattr(e, 'code', 1))
        if code == 0:
            logger.info("Prediction finished successfully.")
        else:
            logger.error("Prediction exited with non-zero code: %s", code)
            sys.exit(code)
    except Exception as e:
        logger.exception("Prediction failed: %s", e)
        sys.exit(1)


if __name__ == '__main__':
    main()
