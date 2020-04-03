cmake_minimum_required(VERSION 3.13)
project(magmatic)

#GLSL compiler support
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
enable_language(GLSL)

#Required libraries
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

#Logger
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

#Utils
find_package(range-v3 REQUIRED)

include_directories("include")


add_library(magmatic-render SHARED "")

#Use external fmt
target_compile_definitions(magmatic-render PRIVATE SPDLOG_FMT_EXTERNAL=1)

set_property(TARGET magmatic-render PROPERTY CXX_STANDARD 20)
target_include_directories(magmatic-render PRIVATE Vulkan::Vulkan)
target_link_libraries(magmatic-render PRIVATE glfw Vulkan::Vulkan spdlog)

add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("examples")