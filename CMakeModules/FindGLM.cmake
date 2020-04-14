#  GLM_FOUND - System has VorbisFile
#  GLM_INCLUDE_DIRS - The VorbisFile include directories
#  GLM_LIBRARIES - The libraries needed to use VorbisFile

find_package(PkgConfig)
pkg_check_modules(PC_glm QUIET glm)

find_path(
		glm_INCLUDE_DIR
		glm/glm.hpp
		HINTS ${PC_glm_INCLUDEDIR} ${PC_glm_INCLUDE_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		glm
		DEFAULT_MSG
		glm_INCLUDE_DIR
)

if(glm_FOUND)
	set(glm_INCLUDE_DIRS ${glm_INCLUDE_DIR})

	if(NOT TARGET glm::glm)
		add_library(glm::glm INTERFACE IMPORTED)
		set_target_properties(
				glm::glm
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${glm_INCLUDE_DIR}
		)
	endif()
endif()

mark_as_advanced(glm_INCLUDE_DIR)