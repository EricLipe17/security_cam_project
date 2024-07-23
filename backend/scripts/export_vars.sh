#!/bin/bash

# PROJECT VARS
echo 'export PROJ_BACKEND=$(dirname $PWD)' >> ~/.bashrc
echo 'export THIRD_PARTY=$PROJ_BACKEND/third-party' >> ~/.bashrc

# FFMPEG
echo 'export FFMPEG_HOME="$THIRD_PARTY/ffmpeg"' >> ~/.bashrc
echo 'export FFMPEG_BUILD="$FFMPEG_HOME/build"' >> ~/.bashrc
echo 'export FFMPEG_BIN="$FFMPEG_BUILD/bin"' >> ~/.bashrc
echo 'export PATH="$FFMPEG_BIN:$PATH"' >> ~/.bashrc

# gSoap
echo 'export GSOAP_HOME="$THIRD_PARTY/gsoap-2.8"' >> ~/.bashrc

# NVENC
echo 'export NVENC_HOME="$THIRD_PARTY/nvenc"' >> ~/.bashrc

# PKG_CONFIG
echo 'export PKG_CONFIG_PATH="$FFMPEG_BUILD/lib/pkgconfig"' >> ~/.bashrc

# Library Path
echo 'export LD_LIBRARY_PATH="$FFMPEG_BUILD/lib:$LD_LIBRARY_PATH"' >> ~/.bashrc

echo .
echo .
echo PLEASE SOURCE: ~/.bashrc
