cmake_minimum_required(VERSION 3.28)
project(renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /O2)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if (WIN32)
        add_compile_options(-O3 -DNDEBUG)
    elseif(UNIX)
        add_compile_options(-Wall -Wextra -Wpedantic)

        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            add_compile_options(-O3 -DNDEBUG)
        endif()

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_compile_options(-ggdb -O0 -fsanitize=address,undefined -fno-omit-frame-pointer)
            add_link_options(-fsanitize=address,undefined)
        endif()
    endif()
endif()

include_directories(
    src/application
    src/camera
    src/parsers
    src/renderer
    src/scene
    src/screen
    src/utils
)

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
    src/application/application.cpp
    src/camera/camera.cpp
    src/parsers/cmd_parser.cpp
    src/parsers/obj_parser.cpp
    src/renderer/renderer.cpp
    src/renderer/filled_renderer.cpp
    src/renderer/wireframe_renderer.cpp
    src/scene/object.cpp
    src/scene/world.cpp
    src/screen/screen.cpp
    src/utils/color.cpp
    src/utils/except.cpp
    src/utils/timer.cpp
)

target_link_libraries(renderer PRIVATE SFML::Graphics Eigen3::Eigen)
