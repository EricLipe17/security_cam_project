cmake_minimum_required(VERSION 3.22)
project(av VERSION 0.0.1 LANGUAGES C CXX)


set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)


# TODO: Figure out why Clion is not passing in the appropriate environment variable for cmake to use
#set(ENV{PKG_CONFIG_PATH} $ENV{FFMPEG_HOME}/build/lib/pkgconfig)
set(ENV{PKG_CONFIG_PATH} /home/eric/projects/security_cam_project/backend/third-party/ffmpeg/build/lib/pkgconfig)
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

set(HEADERS include/Demuxer.hpp
        include/Muxer.hpp
        )

set(SRC src/Demuxer.cpp
        src/Muxer.cpp
        )

add_library(${PROJECT_NAME} STATIC
        ${SRC}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME}  PRIVATE
        ./include
        /home/eric/projects/security_cam_project/backend/third-party/boost_1_86_0
)

target_link_libraries(${PROJECT_NAME}
        PkgConfig::ffmpeg
)
