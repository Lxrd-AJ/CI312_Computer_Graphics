cmake_minimum_required(VERSION 3.6)
project(tutorials)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES Point3D.h)
set(SOURCES Point3D.cpp)
set(SOURCE_FILES main.cpp ${HEADER_FILES} ${SOURCES})
add_executable(tutorials ${SOURCE_FILES})