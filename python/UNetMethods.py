import yaml

def get_config_train(paths_train, paths_val, 
                     checkpoint_dir,
                     label_input='x', label_output='y', 
                     num_workers=0,
                     patch_shape=[80, 80, 80], stride_shape=[40, 40, 40],
                     device='cuda',
                     batchSize=1,
                     resume=None):
    config = {
        "device": device,
        "model": {
            "name": "UNet3D",
            "in_channels": 6,
            "out_channels": 1,
            "layer_order": "crg",
            "f_maps": 32,
            "num_groups": 8,
            "final_sigmoid": True
        },
        "loss": {
            "name": "BCEDiceLoss",
            "ignore_index": None,
            "skip_last_target": True
        },
        "optimizer": {
            "learning_rate": 0.0002,
            "weight_decay": 0.00001
        },
        "eval_metric": {
            "name": "BoundaryAdaptedRandError",
            "threshold": 0.4,
            "use_last_target": True,
            "use_first_input": True
        },
        "lr_scheduler": {
            "name": "ReduceLROnPlateau",
            "mode": "min",
            "factor": 0.5,
            "patience": 30
        },
        "trainer": {
            "eval_score_higher_is_better": False,
            "checkpoint_dir": checkpoint_dir,
            "resume": resume,
            "pre_trained": None,
            "validate_after_iters": 100,
            "log_after_iters": 100,
            "max_num_epochs": 1000,
            "max_num_iterations": 150000,
        },
        "loaders": {
            "num_workers": num_workers,
            "raw_internal_path": "/"+label_input,
            "label_internal_path": "/"+label_output,
            "batch_size": batchSize,
            "train": {
                "file_paths": paths_train,
                "slice_builder": {
                    "name": "SliceBuilder", #"FilterSliceBuilder",
                    "patch_shape": patch_shape,
                    "stride_shape": stride_shape,
                    "skip_shape_check": True,
                    "threshold": 0.6,
                    "slack_acceptance": 0.01
                },
                "transformer": {
                    "raw": [
                        {"name": "Standardize"},
                        {"name": "RandomFlip"},
                        {"name": "RandomRotate90"},
                        {
                            "name": "RandomRotate",
                            "axes": [[2, 1]],
                            "angle_spectrum": 45,
                            "mode": "reflect"
                        },
                        {"name": "ElasticDeformation", "spline_order": 3},
                        {"name": "GaussianBlur3D", "execution_probability": 0.5},
                        {"name": "AdditiveGaussianNoise", "execution_probability": 0.2},
                        {"name": "AdditivePoissonNoise", "execution_probability": 0.2},
                        {"name": "ToTensor", "expand_dims": True}
                    ],
                    "label": [
                        {"name": "RandomFlip"},
                        {"name": "RandomRotate90"},
                        {
                            "name": "RandomRotate",
                            "axes": [[2, 1]],
                            "angle_spectrum": 45,
                            "mode": "reflect"
                        },
                        {"name": "ElasticDeformation", "spline_order": 0},
                        {"name": "StandardLabelToBoundary", "append_label": True},
                        {"name": "ToTensor", "expand_dims": False}
                    ]
                }
            },
            "val": {
                "file_paths": paths_val,
                "slice_builder": {
                    "name": "SliceBuilder", #"FilterSliceBuilder",
                    "patch_shape": patch_shape,
                    "stride_shape": stride_shape,
                    "skip_shape_check": True,
                    "threshold": 0.6,
                    "slack_acceptance": 0.01
                },
                "transformer": {
                    "raw": [
                        {"name": "Standardize"},
                        {"name": "ToTensor", "expand_dims": True}
                    ],
                    "label": [
                        {"name": "StandardLabelToBoundary", "append_label": True},
                        {"name": "ToTensor", "expand_dims": False}
                    ]
                }
            }
        }
    }

    return config

def save_config(config, path):
    with open(path, 'w') as f:
        yaml.dump(config, f)

def get_config_predict(paths_test, model_path, output_dir, num_workers=0, batchSize=1, patch_shape=[40, 40, 40], stride_shape=[40, 40, 40], label_input='x', label_output='y', device='cpu'):
    config = {
        "device": device,
        "model_path": model_path,
        "model": {
            "name": "UNet3D",
            "in_channels": 6,
            "out_channels": 1,
            "layer_order": "crg",
            "f_maps": 32,
            "num_groups": 8,
            "final_sigmoid": True,
        },
        "predictor": {
            "name": "StandardPredictor",
        },
        "loaders": {
            "output_dir": output_dir,
            "batch_size": batchSize,
            "raw_internal_path": "/"+label_input,
            "label_internal_path": "/"+label_output,
            "num_workers": num_workers,
            "test": {
                "file_paths": [f for f in paths_test],
                "slice_builder": {
                    "name": "SliceBuilder",
                    "patch_shape": patch_shape,
                    "stride_shape": stride_shape,
                    "skip_shape_check": True,
                },
                "transformer": {
                    "raw": [
                        {"name": "Standardize"},
                        {"name": "ToTensor", "expand_dims": True},
                    ],
                },
            },
        },
    }
    return config