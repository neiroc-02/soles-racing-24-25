#!/bin/bash

#Installs necessary packages for linux systems (using Debian)
packages=("mosquitto" "mosquitto-clients" "libglfw3-dev" "libglfw3" "xorg-dev" "libxcb1-dev")
for str in ${packages[@]}; 
do
	sudo apt install $str
done