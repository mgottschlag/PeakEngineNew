
cmake_minimum_required(VERSION 2.6)
project(PeakGraphics)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

if(WIN32)
	set(LUA_INCLUDE_DIR C:\\Lua\\5.1\\include)
	set(LUA_LIBRARIES lua51)
else(WIN32)
	find_package(Lua51)
endif(WIN32)

set(SRC
src/core/Graphics.cpp
src/core/GraphicsEntityComponent.cpp
src/core/GraphicsEntityComponentFactory.cpp
src/core/GraphicsScriptBinding.cpp
src/core/GraphicsWorldComponent.cpp
src/core/GraphicsWorldComponentFactory.cpp
src/scene/CameraSceneNode.cpp
src/scene/GroupSceneNode.cpp
src/scene/LightSceneNode.cpp
src/scene/ModelSceneNode.cpp
src/scene/RootSceneNode.cpp
src/scene/SceneNode.cpp
)

include_directories(include ../../engine/include ${LUA_INCLUDE_DIR} /usr/local/include/horde3d)

add_library(PeakGraphics SHARED ${SRC})
target_link_libraries(PeakGraphics Horde3D Horde3DUtils)

install(DIRECTORY include/peakgraphics/ DESTINATION include/peakgraphics FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakGraphics.hpp DESTINATION include)
install(TARGETS PeakGraphics LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)

