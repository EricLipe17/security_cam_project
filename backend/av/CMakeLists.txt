cmake_minimum_required(VERSION 3.22)
project(av VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
# TODO: Figure out why Clion is not passing in the appropriate environment variable for cmake to use
#set(ENV{PKG_CONFIG_PATH} $ENV{FFMPEG_HOME}/build/lib/pkgconfig)
set(ENV{PKG_CONFIG_PATH} /home/ericlipe/security_cam_project/backend/av/third-party/ffmpeg/build/lib/pkgconfig)
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

set(HEADERS include/AV.h
        include/AVIO.h
        include/RtspStream.h
        include/AVUtil.h
        include/Transcode.h
        )

set(SRC src/AVIO.c
        src/RtspStream.c
        src/Transcode.c
        include/StreamWriter.h src/StreamWriter.c)

add_library(${PROJECT_NAME} STATIC
        ${SRC}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME}  PRIVATE
        ./include
)

target_link_libraries(${PROJECT_NAME}
        PkgConfig::ffmpeg
)
