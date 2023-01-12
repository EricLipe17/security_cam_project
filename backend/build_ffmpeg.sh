#!/bin/bash

# Install dependencies if they arent already
sudo apt-get update -qq && sudo apt-get upgrade && sudo apt-get -y install \
  autoconf \
  automake \
  build-essential \
  cmake \
  git-core \
  libass-dev \
  libfreetype6-dev \
  libgnutls28-dev \
  libmp3lame-dev \
  libsdl2-dev \
  libtool \
  libva-dev \
  libvdpau-dev \
  libvorbis-dev \
  libxcb1-dev \
  libxcb-shm0-dev \
  libxcb-xfixes0-dev \
  meson \
  ninja-build \
  pkg-config \
  texinfo \
  wget \
  yasm \
  zlib1g-dev

# Make FFMPEG source and bin directories
mkdir -p $SINGULARITY_FFMPEG $SINGULARITY_FFMPEG_BIN

# Setup & install FFMPEG dependencies
sudo apt install libunistring-dev libaom-dev libdav1d-dev

# NASM
cd $SINGULARITY_FFMPEG && \
cd nasm-2.15.05 && \
./autogen.sh && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" ./configure --prefix="$SINGULARITY_FFMPEG_BUILD" --bindir="$SINGULARITY_FFMPEG_BIN/bin" && \
make && \
make install

# libx264
sudo apt-get install libx264-dev

cd $SINGULARITY_FFMPEG && \
cd x264 && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" PKG_CONFIG_PATH="$SINGULARITY_FFMPEG_BUILD/lib/pkgconfig" ./configure --prefix="$SINGULARITY_FFMPEG_BUILD" --bindir="$SINGULARITY_FFMPEG_BIN/bin" --enable-static --enable-pic && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" make && \
make install

# libx265
sudo apt-get install libx265-dev libnuma-dev

sudo apt-get install libnuma-dev && \
cd $SINGULARITY_FFMPEG && \
cd multicoreware*/build/linux && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$SINGULARITY_FFMPEG_BUILD" -DENABLE_SHARED=off ../../source && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" make && \
make install

# libvpx
cd $SINGULARITY_FFMPEG && \
cd libvpx && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" ./configure --prefix="$SINGULARITY_FFMPEG_BUILD" --disable-examples --disable-unit-tests --enable-vp9-highbitdepth --as=yasm && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" make && \
make install

# libfdk-aac
cd $SINGULARITY_FFMPEG && \
cd fdk-aac && \
autoreconf -fiv && \
./configure --prefix="$SINGULARITY_FFMPEG_BUILD" --disable-shared && \
make && \
make install

# libopus
cd $SINGULARITY_FFMPEG && \
cd opus && \
./autogen.sh && \
./configure --prefix="$SINGULARITY_FFMPEG_BUILD" --disable-shared && \
make && \
make install

# libaom
cd $SINGULARITY_FFMPEG && \
mkdir -p aom_build && \
cd aom_build && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$SINGULARITY_FFMPEG_BUILD" -DENABLE_TESTS=OFF -DENABLE_NASM=on ../aom && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" make && \
make install

# libsvtav1
cd $SINGULARITY_FFMPEG && \
mkdir -p SVT-AV1/build && \
cd SVT-AV1/build && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="$SINGULARITY_FFMPEG_BUILD" -DCMAKE_BUILD_TYPE=Release -DBUILD_DEC=OFF -DBUILD_SHARED_LIBS=OFF .. && \
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" make && \
make install

# libdav1d
cd $SINGULARITY_FFMPEG && \
mkdir -p dav1d/build && \
cd dav1d/build && \
meson setup -Denable_tools=false -Denable_tests=false --default-library=static .. --prefix "$SINGULARITY_FFMPEG_BUILD" --libdir="$SINGULARITY_FFMPEG_BUILD/lib" && \
ninja && \
ninja install

# libvmaf
cd $SINGULARITY_FFMPEG && \
mkdir -p vmaf-2.1.1/libvmaf/build &&\
cd vmaf-2.1.1/libvmaf/build && \
meson setup -Denable_tests=false -Denable_docs=false --buildtype=release --default-library=static .. --prefix "$SINGULARITY_FFMPEG_BUILD" --bindir="$SINGULARITY_FFMPEG_BIN/bin" --libdir="$SINGULARITY_FFMPEG_BUILD/lib" && \
ninja && \
ninja install

# NVIDIA Codec headers
cd SINGULARITY_AV_THIRD_PARTY/nv-codec
make
sudo make install

# Build FFMPEG
cd $SINGULARITY_FFMPEG
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" PKG_CONFIG_PATH="$SINGULARITY_FFMPEG_BUILD/lib/pkgconfig" ./configure \
  --prefix="$SINGULARITY_FFMPEG_BUILD" \
  --pkg-config-flags="--static" \
  --extra-cflags="-I$SINGULARITY_FFMPEG_BUILD/include" \
  --extra-ldflags="-L$SINGULARITY_FFMPEG_BUILD/lib" \
  --extra-libs="-lpthread -lm" \
  --ld="g++" \
  --bindir="$SINGULARITY_FFMPEG_BIN" \
  --enable-gpl \
  --enable-gnutls \
  --enable-libaom \
  --enable-libass \
  --enable-libfdk-aac \
  --enable-libfreetype \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libsvtav1 \
  --enable-libdav1d \
  --enable-libvorbis \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --enable-nonfree \
  --enable-demuxer=rtsp \
  --enable-nvenc &&
PATH="$SINGULARITY_FFMPEG_BIN:$PATH" make && \
make install && \
hash -r
