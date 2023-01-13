#!/bin/bash

# SINGULARITY
echo 'export PROJ_HOME=$(pwd)' >> ~/.bashrc
echo 'export AV_HOME="$PROJ_HOME/av"' >> ~/.bashrc
echo 'export GLFW_HOME="$PROJ_HOME/desktop"' >> ~/.bashrc

echo 'export AV_THIRD_PARTY=$AV_HOME/third-party' >> ~/.bashrc
echo 'export GLFW_THIRD_PARTY=$GLFW_HOME/third-party' >> ~/.bashrc

# FFMPEG
echo 'export FFMPEG_HOME="$AV_THIRD_PARTY/ffmpeg"' >> ~/.bashrc
echo 'export FFMPEG_BUILD="$FFMPEG_HOME/build"' >> ~/.bashrc
echo 'export FFMPEG_BIN="$FFMPEG_HOME/bin"' >> ~/.bashrc

# GLEW
echo 'export GLEW_DEST="$GLFW_THIRD_PARTY/glew"' >> ~/.bashrc

# PKG_CONFIG
echo 'export PKG_CONFIG_PATH="$FFMPEG_BUILD/lib/pkgconfig"' >> ~/.bashrc

echo .
echo .
echo PLEASE SOURCE: ~/.bashrc
