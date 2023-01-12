#!/bin/bash

# SINGULARITY
echo 'export SINGULARITY_HOME=$(pwd)' >> ~/.bashrc
echo 'export SINGULARITY_AV_HOME="$SINGULARITY_HOME/av"' >> ~/.bashrc
echo 'export SINGULARITY_GLFW_HOME="$SINGULARITY_HOME/desktop"' >> ~/.bashrc

echo 'export SINGULARITY_AV_THIRD_PARTY=$SINGULARITY_AV_HOME/third-party' >> ~/.bashrc
echo 'export SINGULARITY_GLFW_THIRD_PARTY=$SINGULARITY_GLFW_HOME/third-party' >> ~/.bashrc

# FFMPEG
echo 'export SINGULARITY_FFMPEG="$SINGULARITY_AV_THIRD_PARTY/ffmpeg"' >> ~/.bashrc
echo 'export SINGULARITY_FFMPEG_BUILD="$SINGULARITY_FFMPEG/build"' >> ~/.bashrc
echo 'export SINGULARITY_FFMPEG_BIN="$SINGULARITY_FFMPEG/bin"' >> ~/.bashrc

# GLEW
echo 'export GLEW_DEST="$SINGULARITY_GLFW_THIRD_PARTY/glew"' >> ~/.bashrc

# PKG_CONFIG
echo 'export PKG_CONFIG_PATH="$SINGULARITY_FFMPEG_BUILD/lib/pkgconfig"' >> ~/.bashrc

echo .
echo .
echo PLEASE SOURCE: ~/.bashrc
