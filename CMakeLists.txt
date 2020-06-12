cmake_minimum_required(VERSION 3.13)
project(magmatic)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#GLSL compiler support
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")
Enable_language(GLSL)

#Required libraries
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(GLM REQUIRED)
find_package(stb REQUIRED)

#Logger
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

#Audio
find_package(OpenAL REQUIRED)
find_package(VorbisFile REQUIRED)
find_package(OpusFile REQUIRED)
find_package(TinyGLTF REQUIRED)
#Table display
find_package(tabulate REQUIRED)

include_directories("include")


#Executables
add_custom_target(examples)
add_executable(main-test "")
add_executable(device_enumeration "")
add_executable(sound_test "")
add_executable(model_info "")
add_dependencies(examples main-test device_enumeration sound_test model_info)

#magmatic-utils
add_library(magmatic-utils SHARED "")
target_compile_definitions(magmatic-utils PRIVATE SPDLOG_FMT_EXTERNAL=1)
target_link_libraries(magmatic-utils PUBLIC spdlog::spdlog_header_only)


#magmatic-render
add_library(magmatic-render SHARED "")
target_compile_definitions(magmatic-render PRIVATE SPDLOG_FMT_EXTERNAL=1 GLM_FORCE_DEPTH_ZERO_TO_ONE)
target_include_directories(magmatic-render PUBLIC Vulkan::Vulkan)
target_link_libraries(magmatic-render PUBLIC glfw Vulkan::Vulkan spdlog::spdlog_header_only glm::glm)
target_link_libraries(magmatic-render PRIVATE TinyGLTF::TinyGLTF)
set_target_properties(magmatic-render PROPERTIES FOLDER modules)

#magmatic-sound
add_library(magmatic-sound SHARED "")
target_link_libraries(magmatic-sound PRIVATE ${OPENAL_LIBRARY} spdlog::spdlog_header_only)
target_link_libraries(magmatic-sound PUBLIC VorbisFile::VorbisFile OpusFile::OpusFile)
target_include_directories(magmatic-sound PUBLIC ${OPENAL_INCLUDE_DIRS})
target_compile_definitions(magmatic-sound PRIVATE SPDLOG_FMT_EXTERNAL=1)
set_target_properties(magmatic-sound PROPERTIES FOLDER modules)

#magmatic-network
add_library(magmatic-network SHARED "")
set_target_properties(magmatic-network PROPERTIES FOLDER modules)

#magmatic-ecs
add_library(magmatic-ecs SHARED "")
set_target_properties(magmatic-ecs PROPERTIES FOLDER modules)

target_link_libraries(main-test PRIVATE magmatic-render magmatic-sound)

add_subdirectory("src")
add_subdirectory("include")
add_subdirectory("examples")
