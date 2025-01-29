cmake_minimum_required(VERSION 3.28)
project(renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -DNDEBUG")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif()

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
    src/camera/camera.cpp
    src/parser/obj_parser.cpp
    src/renderer/renderer.cpp
    src/scene/object.cpp
    src/scene/world.cpp
    src/screen/screen.cpp
)

target_link_libraries(renderer PRIVATE SFML::Graphics Eigen3::Eigen)
