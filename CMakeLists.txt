cmake_minimum_required(VERSION 3.13)
project(magmatic)

set(CMAKE_CXX_STANDARD 17)

#GLSL compiler support
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
enable_language(GLSL)

add_executable(magmatic main.cpp)