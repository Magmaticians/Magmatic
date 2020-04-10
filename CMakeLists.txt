cmake_minimum_required(VERSION 3.13)
project(magmatic)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
set(CMAKE_CXX_STANDARD 20)

#GLSL compiler support
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
Enable_language(GLSL)

#Required libraries
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

#Logger
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

#Table display
find_package(tabulate REQUIRED)

include_directories("include")

add_library(magmatic-render SHARED "")

#Executables
add_custom_target(examples)
add_executable(main-test "")
add_executable(device_enumeration "")
add_dependencies(examples main-test)

#Use external fmt
target_compile_definitions(magmatic-render PRIVATE SPDLOG_FMT_EXTERNAL=1)

set_property(TARGET magmatic-render PROPERTY CXX_STANDARD 20)
target_include_directories(magmatic-render PUBLIC Vulkan::Vulkan)
target_link_libraries(magmatic-render PUBLIC glfw Vulkan::Vulkan spdlog::spdlog_header_only)

target_link_libraries(main-test PRIVATE magmatic-render)

add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("examples")