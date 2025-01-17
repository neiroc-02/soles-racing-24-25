# Base image for ubuntu
FROM ubuntu:latest

# Set my working directory to be project
WORKDIR /GUI

# Copy the GUI folder
COPY . .

# Run necessary updates 
RUN apt-get update && apt-get upgrade -y 

# Extra dependencies for Docker image
RUN apt-get install -y make \
     build-essential \
     libx11-dev \
     libxtst-dev \
     libgl1 \
     xauth \
     libxcb1 \
     libxau6 \
     libxdmcp6 \
     libbsd0 \
     libmd0 

# Install the depedencies
RUN chmod +x some-installs.sh && ./some-installs.sh

# Setting the environment variable
ENV DISPLAY=:0

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
CMD ["/GUI/soles_racing"]


