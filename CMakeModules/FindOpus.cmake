#  OpusFile_FOUND - System has Opus
#  OpusFile_INCLUDE_DIRS - The Opus include directories
#  OpusFile_LIBRARIES - The libraries needed to use Opus

find_package(PkgConfig QUIET)
pkg_check_modules(PC_Opus QUIET opus)

find_path(
		Opus_INCLUDE_DIR
		opus.h
		PATH_SUFFIXES opus
		HINTS ${PC_Opus_INCLUDEDIR} ${PC_Opus_INCLUDE_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		Opus
		DEFAULT_MSG
		Opus_INCLUDE_DIR
)

if(Opus_FOUND)
	set(Opus_INCLUDE_DIRS ${Opus_INCLUDE_DIR})
	if(NOT TARGET Opus::Opus)
		add_library(Opus::Opus INTERFACE IMPORTED)
		set_target_properties(
				Opus::Opus
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${Opus_INCLUDE_DIR}
		)
	endif()
endif()

mark_as_advanced(Opus_INCLUDE_DIR)
