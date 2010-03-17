
cmake_minimum_required(VERSION 2.6)
project(PeakEngine)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(CMAKE_BUILD_TYPE debug)

# Engine

add_subdirectory(engine)

# Plugins

add_subdirectory(plugins/graphics)
add_subdirectory(plugins/network)
add_subdirectory(plugins/physics)
add_subdirectory(plugins/sound)

# Tools

add_subdirectory(tools/runner)
