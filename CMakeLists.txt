cmake_minimum_required (VERSION 3.0)
project (xorgl)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES src/*.cpp src/*.h)

add_executable(xorgl ${SOURCES})

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

#include_directories(${OPENGL_INCLUDE_DIR})

target_link_libraries(xorgl glfw ${OPENGL_gl_LIBRARY})
