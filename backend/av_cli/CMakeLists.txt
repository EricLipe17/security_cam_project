cmake_minimum_required(VERSION 3.22)
project(av_cli VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(HEADERS)

set(SRC src/main.cpp)

add_executable(${PROJECT_NAME}
        ${SRC}
        ${HEADERS}
        )

target_include_directories(${PROJECT_NAME}  PRIVATE
        $ENV{PROJ_BACKEND}/av/include
        $ENV{BOOST_HOME}
        )

target_link_libraries(${PROJECT_NAME}
        av
        )
