cmake_minimum_required(VERSION 3.10)
project(renderer)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(src/lib/eigen)

add_executable(
    renderer
    src/main.cpp
    src/renderer.cpp
    src/object.cpp
)
