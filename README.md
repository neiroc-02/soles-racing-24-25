# SOLES RACING PROJECT CODE
## Description
This is the repository for all the code for SOLES Racing, regarding the Arduino Code and the GUI code.

## Installation

### Windows
Run commands through MSYS2 MINGW64 shell (make sure you are in the GUI folder)

```
pacman -S --noconfirm mingw-w64-x86_64-gcc
pacman -S --noconfirm mingw-w64-x86_64-make
pacman -S --noconfirm mingw-w64-x86_64-pkg-config
pacman -S --noconfirm mingw-w64-x86_64-glfw
```

```
make
```

```
./soles-racing
```

### Linux 
#### Setting the GUI up:
1. Download all dependencies: `./some_installs.sh`
2. Then create the executable: `make`
3. Then run the executable: `./soles_racing`

#### Setting up the mosquitto server:
1. Install mosquitto dependencies (listed in `./some_installs.sh`)
2. Create a mosquitto.conf file: `sudo vim /etc/mosquitto/mosquitto.conf`
3. Build the mosquitto.conf file as follows:
```
pid_file /run/mosquitto/mosquitto.pid

persistence true
persistence_location /var/lib/mosquitto/

log_dest topic

log_type error
log_type warning
log_type notice
log_type information

bind_address 0.0.0.0

connection_messages true
log_timestamp true
allow_anonymous true
listener 1883

include_dir /etc/mosquitto/conf.d
```
4. Start the mosquitto server with the following command: `mosquitto -v -c /etc/mosquitto/mosquitto.conf`

### Mac
TODO

## Usage 
