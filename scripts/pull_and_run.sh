#!/bin/bash

set -euxo pipefail

ssh -t pirate@172.20.10.14 'docker login && docker pull dmahon10/adversarial_chair:latest && docker run -it --net=host --ipc=host --env=DISPLAY=$DISPLAY -v /tmp/.X11-unix:/.X11-unix --device /dev/video0:/dev/video0 --device /dev/gpiomem dmahon10/adversarial_chair:latest'
