#  GLM_FOUND - System has VorbisFile
#  GLM_INCLUDE_DIRS - The VorbisFile include directories
#  GLM_LIBRARIES - The libraries needed to use VorbisFile

find_package(PkgConfig)
pkg_check_modules(PC_GLM QUIET glm)

find_path(
		GLM_INCLUDE_DIRS
		glm/glm.hpp
		HINTS ${PC_GLM_INCLUDEDIR} ${PC_GLM_INCLUDE_DIRS}
)

find_library(
		GLM_LIBRARY
		NAMES libglm glm
		HINTS ${PC_GLM_LIBDIR} ${PC_GLM_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		GLM
		DEFAULT_MSG
		GLM_LIBRARY
		GLM_INCLUDE_DIR
)

if(GLM_FOUND)
	set(GLM_LIBRARIES ${GLM_LIBRARY})
	set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})

	if(NOT TARGET glm::glm)
		add_library(glm::glm UNKNOWN IMPORTED)
		set_target_properties(
				glm::glm
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${GLM_INCLUDE_DIR}
				IMPORTED_LOCATION ${GLM_LIBRARY}
		)
	endif()
endif()

mark_as_advanced(GLM_LIBRARY GLM_INCLUDE_DIR)
