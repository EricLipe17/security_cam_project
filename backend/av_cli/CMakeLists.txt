cmake_minimum_required(VERSION 3.22)
project(av_cli VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

set(HEADERS)

set(SRC src/main.c)

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
        )
