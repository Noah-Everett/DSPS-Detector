


docker run --gpus all -d --name test-pyt \
  -u $(id -u):$(id -g) -e HOME=$HOME -e USER=$USER -v $HOME:$HOME \
  nvcr.io/nvidia/pytorch:24.05-py3