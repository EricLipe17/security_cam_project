cmake_minimum_required(VERSION 3.22)
project(singularity-prog_test VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

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
        singularity-av
        )