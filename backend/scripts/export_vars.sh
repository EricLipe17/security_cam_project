#!/bin/bash

# PROJECT VARS
export PROJ_BACKEND=$(dirname $PWD)
export THIRD_PARTY=$PROJ_BACKEND/third-party

# Boost
export BOOST_HOME="$THIRD_PARTY/boost_1_86_0"
export BOOST_BUILD="$BOOST_HOME/build"
export BOOST_INSTALL="$BOOST_BUILD/install"
export BOOST_LIB="$BOOST_BUILD/lib"
export BOOST_INCLUDE="$BOOST_BUILD/include"
export PATH="$BOOST_BIN:$PATH"

# FFMPEG
export FFMPEG_HOME="$THIRD_PARTY/ffmpeg"
export FFMPEG_BUILD="$FFMPEG_HOME/build"
export FFMPEG_BIN="$FFMPEG_BUILD/bin"
export PATH="$FFMPEG_BIN:$PATH"

# gSoap
export GSOAP_HOME="$THIRD_PARTY/gsoap-2.8"

# NVENC
export NVENC_HOME="$THIRD_PARTY/nvenc"

# PKG_CONFIG
export PKG_CONFIG_PATH="$FFMPEG_BUILD/lib/pkgconfig"

# Library Path
export LD_LIBRARY_PATH="$FFMPEG_BUILD/lib:$LD_LIBRARY_PATH"

