cmake_minimum_required(VERSION 3.22)
project(gui VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

set(CMAKE_C_STANDARD 17)

add_subdirectory(third-party/glfw)

add_library(glew STATIC IMPORTED GLOBAL)
set_target_properties(glew PROPERTIES IMPORTED_LOCATION /usr/lib64/libGLew.a)
set_target_properties(glew PROPERTIES INCLUDE_DIRECTORIES ./third-party/GLEW/include )

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
        glfw
        GL
        GLU
        X11
        GLEW
        av
)