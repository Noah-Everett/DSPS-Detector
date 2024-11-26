docker run --shm-size=8g --gpus all -it --mount type=bind,source=/home/noaheverett,target=/noaheverett noaheverett/dsps:1.1.3-x86-Cuda

python train_UNet.py --data-dir /noaheverett/MLdataNumbers/ --output-dir /noaheverett/TrainOutput --num-workers 48 --device cuda