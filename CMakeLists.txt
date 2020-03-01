cmake_minimum_required(VERSION 3.13)
project(magmatic)

#GLSL compiler support
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
enable_language(GLSL)

#Required libraries
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

include_directories("include")

add_library(magmatic-render SHARED "")
target_link_libraries(magmatic-render PRIVATE glfw Vulkan::Vulkan)
target_compile_features(magmatic-render PUBLIC cxx_std_17)

add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("examples")