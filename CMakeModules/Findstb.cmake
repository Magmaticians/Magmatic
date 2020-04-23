#  GLM_FOUND - System has glm
#  GLM_INCLUDE_DIRS - The glm include directories
#  GLM_LIBRARIES - The libraries needed to use glm


find_path(
		stb_INCLUDE_DIR
		stb/stb_image.h
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		stb
		DEFAULT_MSG
		stb_INCLUDE_DIR
)

if(stb_FOUND)
	set(stb_INCLUDE_DIRS ${stb_INCLUDE_DIR})

	if(NOT TARGET stb::stb)
		add_library(stb::stb INTERFACE IMPORTED)
		set_target_properties(
				stb::stb
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${stb_INCLUDE_DIR}
		)
	endif()
endif()

mark_as_advanced(stb_INCLUDE_DIR)