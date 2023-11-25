#!/bin/bash

# Setup environment variables
source ./export_vars.sh

sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev
cd $GLFW_THIRD_PARTY/glew || exit
make
sudo make install
make clean

# Add /usr/lib64 to /etc/ld.so.conf and run `sudo ldconfig`
