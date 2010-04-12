
cmake_minimum_required(VERSION 2.4)
project(PeakRunner)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

if(WIN32)
	set(LUA_INCLUDE_DIR C:\\Lua\\5.1\\include)
	set(LUA_LIBRARIES lua51)
else(WIN32)
	find_package(Lua51)
endif(WIN32)

set(SRC
src/main.cpp
)

include_directories(
include
../../engine/include
../../plugins/physics/include
../../plugins/graphics/include
../../plugins/network/include
${LUA_INCLUDE_DIR}
)

add_executable(PeakRunner ${SRC})
target_link_libraries(PeakRunner Horde3D PeakEngine PeakGraphics PeakNetwork PeakPhysics Xrandr glfw)

set(CMAKE_BUILD_TYPE debug)
