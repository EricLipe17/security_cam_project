cmake_minimum_required(VERSION 3.22)
project(av_cli VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(ENV{PKG_CONFIG_PATH} $ENV{FFMPEG_BUILD}/lib/pkgconfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(ffmpeg REQUIRED IMPORTED_TARGET
        libavdevice
        libavfilter
        libavformat
        libavcodec
        libswresample
        libswscale
        libavutil
)

set(HEADERS)

set(SRC src/main.cpp)

add_executable(${PROJECT_NAME}
        ${SRC}
        ${HEADERS}
        )

target_include_directories(${PROJECT_NAME}  PRIVATE
        ./include
        ../av/include
        )

target_link_libraries(${PROJECT_NAME}
        av
        PkgConfig::ffmpeg
        )
