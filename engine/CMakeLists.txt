
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
src/core/Engine.cpp
src/core/Entity.cpp
src/core/EntityComponent.cpp
src/core/FloatProperty.cpp
src/core/IntProperty.cpp
src/core/Property.cpp
src/core/QuaternionProperty.cpp
src/core/Vector2FProperty.cpp
src/core/Vector3FProperty.cpp
src/core/World.cpp
src/import/ScriptEntityComponent.cpp
src/import/ScriptEntityComponentFactory.cpp
src/import/XMLEntityFactory.cpp
src/import/XMLGame.cpp
src/import/XMLWorld.cpp
src/support/Buffer.cpp
src/support/ConditionVariable.cpp
src/support/EngineScriptBinding.cpp
src/support/Event.cpp
src/support/FrameLimiter.cpp
src/support/Mutex.cpp
src/support/OS.cpp
src/support/ScopedLock.cpp
src/support/Script.cpp
src/support/Thread.cpp
src/support/tinystr.cpp
src/support/tinyxml.cpp
src/support/tinyxmlerror.cpp
src/support/tinyxmlparser.cpp
)

include_directories(include ${LUA_INCLUDE_DIR})

add_library(PeakEngine SHARED ${SRC})
target_link_libraries(PeakEngine luabind ${LUA_LIBRARIES} pthread)

install(DIRECTORY include/peakengine/ DESTINATION include/peakengine FILES_MATCHING PATTERN *.hpp)
install(FILES include/PeakEngine.hpp DESTINATION include)
install(TARGETS PeakEngine LIBRARY DESTINATION lib)

set(CMAKE_BUILD_TYPE debug)
