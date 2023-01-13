#!/bin/bash

pushd $FFMPEG_HOME

# NASM
wget https://www.nasm.us/pub/nasm/releasebuilds/2.15.05/nasm-2.15.05.tar.bz2 && \
tar xjvf nasm-2.15.05.tar.bz2
rm -rf nasm-2.15.05/.git nasm-2.15.05.tar.bz2

# x264
git -C x264 pull 2> /dev/null || git clone --depth 1 https://code.videolan.org/videolan/x264.git
rm -rf x264/.git

# x265
wget -O x265.tar.bz2 https://bitbucket.org/multicoreware/x265_git/get/master.tar.bz2 && \
tar xjvf x265.tar.bz2
mv multicoreware* x265
rm -rf x265/.git x265.tar.bz2

# VPX
git -C libvpx pull 2> /dev/null || git clone --depth 1 https://chromium.googlesource.com/webm/libvpx.git
rm -rf libvpx/.git

# FDK-AAC
git -C fdk-aac pull 2> /dev/null || git clone --depth 1 https://github.com/mstorsjo/fdk-aac
rm -rf fdk-aac/.git

# Opus
git -C opus pull 2> /dev/null || git clone --depth 1 https://github.com/xiph/opus.git
rm -rf opus/.git

# AOM
git -C aom pull 2> /dev/null || git clone --depth 1 https://aomedia.googlesource.com/aom
rm -rf aom/.git

# SVT-AV1
git -C SVT-AV1 pull 2> /dev/null || git clone https://gitlab.com/AOMediaCodec/SVT-AV1.git
rm -rf SVT-AV1/.git

# dav1d
git -C dav1d pull 2> /dev/null || git clone --depth 1 https://code.videolan.org/videolan/dav1d.git
rm -rf dav1d/.git

# VMAF
wget https://github.com/Netflix/vmaf/archive/v2.1.1.tar.gz && tar xvf v2.1.1.tar.gz
rm -rf vmaf-2.1.1/.git v2.1.1.tar.gz

popd