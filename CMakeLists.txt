cmake_minimum_required(VERSION 3.28)
project(renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/include)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

include(FetchContent)
FetchContent_Declare(Eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(Eigen)

add_executable(
    renderer
    src/main.cpp
    src/renderer/renderer.cpp
    src/scene/world.cpp
    src/scene/object.cpp
)

target_link_libraries(renderer PRIVATE SFML::Graphics Eigen3::Eigen)
