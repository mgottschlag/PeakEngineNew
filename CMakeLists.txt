
cmake_minimum_required(VERSION 2.6)
project(PeakEngine)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(CMAKE_BUILD_TYPE debug)

# Engine

add_subdirectory(engine)

# Plugins

add_subdirectory(plugins/graphics)
add_subdirectory(plugins/physics)

# Tools

add_subdirectory(tools/editor)

