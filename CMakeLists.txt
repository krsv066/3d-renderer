cmake_minimum_required(VERSION 3.10)
project(renderer)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_subdirectory(src/lib/SDL)
include_directories(src/lib/eigen src/lib/SDL/include)

add_executable(main src/main.cpp)

target_link_libraries(main SDL3-shared)
