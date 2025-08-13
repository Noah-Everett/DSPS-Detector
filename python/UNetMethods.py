import yaml

def get_config_model():
    return {
        "name": "UNet3D",
        "in_channels": 6,
        "out_channels": 1,
        "layer_order": "crg",
        "f_maps": 32,
        "num_groups": 8,
        "final_sigmoid": True,
    }

def get_config_loaders(
        num_workers,
        label_input,
        label_output,
        batchSize,
        output_dir,
        paths_train,
        paths_val,
        paths_test,
        patch_shape,
        stride_shape,
    ):
    return {
        "num_workers": num_workers,
        "raw_internal_path": "/"+label_input,
        "label_internal_path": "/"+label_output,
        "batch_size": batchSize,
        "output_dir": output_dir,
        "raw_internal_path": "/"+label_input,
        "label_internal_path": "/"+label_output,
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
            "transformer": get_config_testValTransformer(),
        },
        "test": {
            "file_paths": [f for f in paths_test],
            "slice_builder": {
                "name": "SliceBuilder",
                "patch_shape": patch_shape,
                "stride_shape": stride_shape,
                "skip_shape_check": True,
            },
            "transformer": get_config_testValTransformer()
        },
    }

def get_config_testValTransformer():
    return {
        "raw": [
            {"name": "Standardize"},
            {"name": "ToTensor", "expand_dims": True},
        ],
        # "label": [
        #     {"name": "StandardLabelToBoundary", "append_label": True},
        #     {"name": "ToTensor", "expand_dims": False},
        # ]
    }

def get_config_train(paths_train, paths_val, 
                     checkpoint_dir,
                     label_input='x', label_output='y', 
                     num_workers=0,
                     patch_shape=[80, 80, 80], stride_shape=[40, 40, 40],
                     device='cuda',
                     batchSize=1,
                     resume=None,
                     iters_val=100,
                     iters_max=150000,
                     iters_log=100,
                     epochs_max=1000,
                     lr=0.0002,
    ):
    config = {
        "device": device,
        "model": get_config_model(),
        "loss": {
            "name": "BCEDiceLoss",
            "ignore_index": None,
            "skip_last_target": True
        },
        "optimizer": {
            "learning_rate": lr,
            "weight_decay": 0.00001
        },
        "eval_metric": {
            "name": "DiceCoefficient",
            "threshold": 0.9,
            "use_last_target": True,
            "use_first_input": True
        },
        "lr_scheduler": {
            "name": "ReduceLROnPlateau",
            "mode": "max",
            "factor": 0.5,
            "patience": 3
        },
        "trainer": {
            "eval_score_higher_is_better": True,
            "checkpoint_dir": checkpoint_dir,
            "resume": resume,
            "pre_trained": None,
            "validate_after_iters": iters_val,
            "log_after_iters": iters_log,
            "max_num_epochs": epochs_max,
            "max_num_iterations": iters_max,
        },
        "loaders": get_config_loaders(
            num_workers=num_workers,
            label_input=label_input,
            label_output=label_output,
            batchSize=batchSize,
            output_dir=None,
            paths_train=paths_train,
            paths_val=paths_val,
            paths_test=[],
            patch_shape=patch_shape,
            stride_shape=stride_shape
        )
    }

    return config

def get_config_predict(paths_test, model_path, output_dir, num_workers=0, batchSize=1, patch_shape=[80, 80, 80], stride_shape=[40, 40, 40], label_input='x', label_output='y', device='cpu'):
    config = {
        "device": device,
        "model_path": model_path,
        "model": get_config_model(),
        "predictor": {
            "name": "StandardPredictor",
        },
        "loaders": get_config_loaders(
            num_workers=num_workers,
            label_input=label_input,
            label_output=label_output,
            batchSize=batchSize,
            output_dir=output_dir,
            paths_train=[],
            paths_val=[],
            paths_test=paths_test,
            patch_shape=patch_shape,
            stride_shape=stride_shape
        )
    }
    return config

def save_config(config, path):
    with open(path, 'w') as f:
        yaml.dump(config, f)