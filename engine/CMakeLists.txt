
cmake_minimum_required(VERSION 2.6)
project(PeakEngine)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

set(SRC
src/core/World.cpp
src/support/Buffer.cpp
src/support/ConditionVariable.cpp
src/support/FrameLimiter.cpp
src/support/Mutex.cpp
src/support/OS.cpp
src/support/ScopedLock.cpp
src/support/Thread.cpp
)

include_directories(include)

add_library(PeakEngine SHARED ${SRC})

install(DIRECTORY include/peakengine/ DESTINATION include/peakengine FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakEngine.hpp DESTINATION include)
install(TARGETS PeakEngine LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)
