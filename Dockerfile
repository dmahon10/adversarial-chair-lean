# This is Pi Specific (64bit ARM)
FROM mohaseeb/raspberrypi3-python-opencv

ENV ENVIRONMENT rpi

COPY app /usr/local/app
WORKDIR /usr/local/app

# Install requirements
RUN sudo apt-get update \
    && sudo pip install RPi.GPIO

CMD ["python", "videocv2.py"]