cmake_minimum_required(VERSION 3.10)
project(renderer)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
set(SFML_DIR "src/lib/SFML/lib/cmake/SFML")

include_directories(
    src/include
    src/lib/eigen
    src/lib/SFML/include
)

add_executable(
    renderer
    src/main.cpp
    src/renderer/renderer.cpp
    src/scene/object.cpp
)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)
target_link_libraries(renderer PRIVATE SFML::Graphics SFML::Audio SFML::Network)
