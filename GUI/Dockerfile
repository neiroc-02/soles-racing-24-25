# Base image for ubuntu
FROM ubuntu:latest

ENV DISPLAY=:0
#Set my working directory to be project
WORKDIR /GUI

# Copy the GUI folder
COPY . .

# Run necessary updates 
RUN apt-get update && apt-get upgrade -y 

# Extra dependencies for Docker image
RUN apt-get install -y make 
RUN apt-get install -y build-essential

# Install the depedencies
RUN chmod +x some-installs.sh && ./some-installs.sh

# Make the executable 
RUN make all

# Run the executable
CMD ["./soles_racing"]

