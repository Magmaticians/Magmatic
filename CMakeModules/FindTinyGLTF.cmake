#  TinyGLTF_FOUND - System has TinyGLTF
#  TinyGLTF_INCLUDE_DIRS - The TinyGLTF include directories
#  TinyGLTF_LIBRARIES - The libraries needed to use TinyGLTF


find_package(PkgConfig)
pkg_check_modules(PC_TinyGLTF QUIET TinyGLTF)

find_path(
		TinyGLTF_INCLUDE_DIR
		tiny_gltf.h
		HINTS ${PC_TinyGLTF_INCLUDEDIR} ${PC_TinyGLTF_INCLUDE_DIRS}

)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		TinyGLTF
		DEFAULT_MSG
		TinyGLTF_INCLUDE_DIR
)

if(TinyGLTF_FOUND)
	set(TinyGLTF_INCLUDE_DIRS ${TinyGLTF_INCLUDE_DIR})

	if(NOT TARGET TinyGLTF::TinyGLTF)
		add_library(TinyGLTF::TinyGLTF INTERFACE IMPORTED)
		set_target_properties(
				TinyGLTF::TinyGLTF
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${TinyGLTF_INCLUDE_DIR}
		)
	endif()
endif()

mark_as_advanced(TinyGLTF_INCLUDE_DIR)