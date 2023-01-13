#!/bin/bash

# PROJECT VARS
echo 'export PROJ_BACKEND=$(pwd)' >> ~/.bashrc
echo 'export AV_HOME="$PROJ_BACKEND/av"' >> ~/.bashrc
echo 'export GLFW_HOME="$PROJ_BACKEND/desktop"' >> ~/.bashrc

echo 'export AV_THIRD_PARTY=$AV_HOME/third-party' >> ~/.bashrc
echo 'export GLFW_THIRD_PARTY=$GLFW_HOME/third-party' >> ~/.bashrc

# FFMPEG
echo 'export FFMPEG_HOME="$AV_THIRD_PARTY/ffmpeg"' >> ~/.bashrc
echo 'export FFMPEG_BUILD="$FFMPEG_HOME/build"' >> ~/.bashrc
echo 'export FFMPEG_BIN="$FFMPEG_BUILD/bin"' >> ~/.bashrc

# GLEW
echo 'export GLEW_DEST="$GLFW_THIRD_PARTY/glew"' >> ~/.bashrc

# PKG_CONFIG
echo 'export PKG_CONFIG_PATH="$FFMPEG_BUILD/lib/pkgconfig"' >> ~/.bashrc

echo .
echo .
echo PLEASE SOURCE: ~/.bashrc
