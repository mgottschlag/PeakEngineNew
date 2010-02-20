
cmake_minimum_required(VERSION 2.6)
project(PeakPhysics)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

if(WIN32)
	set(LUA_INCLUDE_DIR C:\\Lua\\5.1\\include)
	set(LUA_LIBRARIES lua51)
else(WIN32)
	find_package(Lua51)
endif(WIN32)

set(SRC
src/core/Physics.cpp
src/core/PhysicsEntityComponent.cpp
src/core/PhysicsEntityComponentFactory.cpp
src/core/PhysicsScriptBinding.cpp
src/core/PhysicsWorldComponent.cpp
src/core/PhysicsWorldComponentFactory.cpp
src/physics/Body.cpp
src/physics/Box.cpp
src/physics/Capsule.cpp
src/physics/CharacterController.cpp
src/physics/Compound.cpp
src/physics/Heightfield.cpp
src/physics/PhysicsProfiler.cpp
src/physics/Plane.cpp
src/physics/Shape.cpp
src/physics/Simulation.cpp
src/physics/Sphere.cpp
src/physics/Trimesh.cpp
)

include_directories(include ../../engine/include ${LUA_INCLUDE_DIR} /usr/local/include/bullet)

add_library(PeakPhysics SHARED ${SRC})
#target_link_libraries(PeakPhysics Horde3D Horde3DUtils)

install(DIRECTORY include/peakphysics/ DESTINATION include/peakphysics FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakPhysics.hpp DESTINATION include)
install(TARGETS PeakPhysics LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)

