

<p><iframe frameborder="0" height="315" src="https://www.youtube.com/embed/YnwGxl61sro?controls=0" width="560"></iframe></p>

<h1>&nbsp;</h1>

<p><iframe frameborder="0" height="315" src="https://www.youtube.com/embed/V6gQ_h0EiO8?controls=0" width="560"></iframe></p>



# What is it?

This repo contains the software for a computer vision motor control project. Specifically, the brains of a face following swivel robot. It behaves in much the same as a face tracking security camera, but I want to use it to create a chair that always turns away from you, making it impossible to sit in.

The `app` files run on a Raspberry Pi 4 with a camera module and the `embedded` files run on a `ATTINY44` microcontroller that I have on a custom motor driver circuit.



### Set Up

The Raspberry Pi runs the computer vision logic and sends data out its GPIO pins to the ATTINY. In the C files you will find the **Pulse Width Modulation** logic that runs on the ATTINY, controlling a DC motor.

The camera should be housed in the swiveling chassis driven by the DC motor. 



### Behaviour

When a human face enters the camera's field of view, as long as the face is not centered in the frame, the motor will work to revolve the camera so that it points directly at the face. 

In the case of multiple faces in the frame, the robot will only work to center the camera on the nearest face. 



### Deployment

To facilitate efficient remote deployments I utilized Docker and Docker Hub. All of the Raspberry Pi's firmware is run in a Docker container. In the `scripts` directory you will find bash scripts that streamline `ssh`ing into the Pi, logging into Docker Hub from the Pi and pulling down the latest image. This means that to send out updated code to the robot all I needed to do was push the new image to Docker Hub and restart the Pi (this of course could be scripted as well).



### Challenges

There were several small hurdles caused by using Docker on the Pi. Namely, the difficulty of accessing the Pi's hardware (GPIO pins, display and camera) from within a docker container. Solutions to this issue exist, it was only a matter of piecing them together from sundry sources. The most robust solution I found involved the addition of a few helpful flags to the docker run script.

```bash
docker run -it --net=host --ipc=host --env=DISPLAY=$DISPLAY -v /tmp/.X11-unix:/.X11-unix --device /dev/video0:/dev/video0 --device /dev/gpiomem dmahon10/adversarial_chair:latest'
```

Addition mechanical and electrical challenges are outlined below.



### Results

Some successes, some failures. 

Deployment was fantastic. The only thing I'd add would be a watch tower installation on the Pi so that it could regularly check for newer Docker images to pull and run, rather than having to be rebooted. 

The software logic was sound and performed as hoped. OpenCV competently tracked the nearest face in the frame and performed reasonably well with a range of skin tones. A small issue is that it also tracks faces in photos and paintings. This would not be ideal for the final installation of the robot, but is acceptable for now. 

The ATTINY firmware also worked swimmingly. The PWM implementation controlled motor speed as expected and the motor driver changed the motor's direction without a hiccup. 

Things were not as rosy on the mechanical side, granted I knew it really was going to be an MVP prototype configuration. I didn't use any gearing in the swivel mechanism which turned out to be the biggest issue. When programmed for high speed, the motor always over corrected the camera's position. To combat this I made the motor turn very slowly, but with that came the challenge of getting it to move the chassis at all. At that point I realized this was a mechanical engineering rather than software engineering issue, but as this is a proof of concept *software* project, I can live with the poor physical performance.

Lastly, powering the set up turned out to be a challenge. The motor commanded more amperage than I had lying around and I destroyed a few portable  battery packs in the process. Not after getting a few hours of use out of them, though. 



### Gallery



VIDEO of SCREEN



VIDEO OF ROBOT









