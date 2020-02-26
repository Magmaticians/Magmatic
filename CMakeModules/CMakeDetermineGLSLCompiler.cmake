
if(NOT $ENV{GLSL} STREQUAL "")	
	set(CMAKE_GLSL_COMPILER $ENV{GLSL} CACHE PATH "GLSL Compiler")
else()
	find_program(
		CMAKE_GLSL_COMPILER
			NAMES "glslangValidator"
			DOC "GLSL compiler"
	)
endif()
mark_as_advanced(CMAKE_GLSL_COMPILER)

set(CMAKE_GLSL_SOURCE_FILE_EXTENSIONS vert;VERT;test;TEST;tese;TESE;geom;GEOM;frag;FRAG;comp;COMP)
set(CMAKE_GLSL_OUTPUT_EXTENSION .spv)
set(CMAKE_GLSL_COMPILER_ENV_VAR "GLSL")


configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeGLSLCompiler.cmake.in
		${CMAKE_PLATFORM_INFO_DIR}/CMakeGLSLCompiler.cmake)
