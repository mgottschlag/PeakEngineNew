
project(PeakEditor)

cmake_minimum_required(VERSION 2.4.0)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")

find_package(Qt4 REQUIRED)


set(QT_USE_OPENGL TRUE)
set(QT_USE_QTXML TRUE)
include(${QT_USE_FILE})

set(SRC
src/main.cpp
src/EditorWindow.cpp
src/ObjectListModel.cpp
src/EditorProject.cpp
src/CodeGenerator.cpp
src/OpenGLView.cpp
src/NewObjectDialog.cpp
src/CodeComponentObject.cpp
src/EntityObject.cpp
src/CodeEditor.cpp
src/CodeEditWidget.cpp
src/ProjectSettings.cpp
)

set(MOC_H
include/EditorWindow.hpp
include/ObjectListModel.hpp
include/NewObjectDialog.hpp
include/CodeEditor.hpp
include/EditorObject.hpp
include/CodeComponentObject.hpp
include/CodeEditWidget.hpp
include/ProjectSettings.hpp
)

QT4_WRAP_CPP(MOC_SRC ${MOC_H})

set(QRC
data/editor.qrc
)

QT4_ADD_RESOURCES(QRC_SRC ${QRC})

set(UIS
ui/MainWindow.ui
)

QT4_WRAP_UI(UIS_H ${UIS})

include_directories(include ../../engine/include ${CMAKE_CURRENT_BINARY_DIR} ${QT_QTOPENGL_INCLUDE_DIR})

add_executable(PeakEditor ${SRC} ${UIS_H} ${MOC_SRC} ${QRC_SRC})
target_link_libraries(PeakEditor ${QT_LIBRARIES} ${QT_QTOPENGL_LIBRARIES})

install(TARGETS PeakEditor RUNTIME DESTINATION bin)
