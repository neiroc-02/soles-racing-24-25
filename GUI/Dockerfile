# Base image for ubuntu
FROM ubuntu:latest

#Set my working directory to be project
WORKDIR /GUI

# Copy the GUI folder
COPY ./GUI .

RUN apt-get update && apt-get upgrade

# Install the depedencies
RUN chmod +x some_installs.sh && ./some_installs.sh

# Make the executable
RUN make

# Run the executable
CMD ["./soles_racing"]

