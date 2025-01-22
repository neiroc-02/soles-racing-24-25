# Base image for ubuntu
FROM ubuntu:latest

# Set my working directory to be project
WORKDIR /GUI

# Copy the GUI folder
COPY . .
COPY some-installs.sh /GUI/

# Run necessary updates 
RUN apt-get update && apt-get upgrade -y 

# Extra dependencies for Docker image
RUN apt-get install -y make \
     build-essential \
     libx11-dev \
     libxcursor-dev \
     libxtst-dev \
     libgl1 \
     libglfw3-dev \
     libgl1-mesa-dev \
     libxi-dev \
     libxrandr-dev \
     libxinerama-dev \
     libxxf86vm-dev \
     xauth \
     libxcb1 \
     libxau6 \
     libxdmcp6 \
     libbsd0 \
     libmd0 \
     pkg-config \
     x11-xserver-utils \
     xorg \
     mesa-utils \
     xvfb \
     pulseaudio

# Install the depedencies
RUN chmod +x some-installs.sh && ./some-installs.sh

# Create XDG runtime directory
ENV XDG_RUNTIME_DIR=/tmp/runtime-docker
RUN mkdir -p /tmp/runtime-docker && chmod 700 /tmp/runtime-docker

# Setting the environment variable
ENV DISPLAY=:0
ENV NVIDIA_VISIBLE_DEVICES=all
ENV NVIDIA_DRIVER_CAPABILITIES=all
ENV PULSE_SERVER=unix:/tmp/pulseaudio.socket

# Make the executable 
RUN make all

EXPOSE 8000

# Found on this stack overflow post: https://stackoverflow.com/questions/16296753/can-you-run-gui-applications-in-a-linux-docker-container
# RUN export uid=1000 gid=1000 && \
#     mkdir -p /home/developer && \
#     echo "developer:x:${uid}:${gid}:Developer,,,:/home/developer:/bin/bash" >> /etc/passwd && \
#     echo "developer:x:${uid}:" >> /etc/group && \
#     echo "developer ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/developer && \
#     chmod 0440 /etc/sudoers.d/developer && \
#     chown ${uid}:${gid} -R /home/developer
# USER developer
# ENV HOME /home/developer

# Run the executable
CMD ["xvfb-run", "-a", "/GUI/soles_racing"]


