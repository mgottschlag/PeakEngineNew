
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
src/core/World.cpp
src/support/Buffer.cpp
src/support/ConditionVariable.cpp
src/support/FrameLimiter.cpp
src/support/Mutex.cpp
src/support/OS.cpp
src/support/ScopedLock.cpp
src/support/Script.cpp
src/support/Thread.cpp
)

include_directories(include ${LUA_INCLUDE_DIR})

add_library(PeakEngine SHARED ${SRC})
target_link_libraries(PeakEngine luabind ${LUA_LIBRARIES})

install(DIRECTORY include/peakengine/ DESTINATION include/peakengine FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakEngine.hpp DESTINATION include)
install(TARGETS PeakEngine LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)
