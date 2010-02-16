
cmake_minimum_required(VERSION 2.6)
project(PeakEngine)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

if(WIN32)
	set(LUA_INCLUDE_DIR C:\\Lua\\5.1\\include)
	set(LUA_LIBRARIES lua51)
else(WIN32)
	find_package(Lua51)
endif(WIN32)

set(SRC
src/core/ClientEntityComponent.cpp
src/core/ClientWorldComponent.cpp
src/core/Network.cpp
src/core/NetworkEntityComponent.cpp
src/core/NetworkScriptBinding.cpp
src/core/NetworkWorldComponent.cpp
src/core/ServerEntityComponent.cpp
src/core/ServerWorldComponent.cpp
src/network/NetworkClient.cpp
src/network/NetworkConnection.cpp
src/network/NetworkHost.cpp
)

include_directories(include ../../engine/include ${LUA_INCLUDE_DIR})

add_library(PeakNetwork SHARED ${SRC})
target_link_libraries(PeakNetwork enet)

install(DIRECTORY include/peaknetwork/ DESTINATION include/peaknetwork FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakNetwork.hpp DESTINATION include)
install(TARGETS PeakNetwork LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)

