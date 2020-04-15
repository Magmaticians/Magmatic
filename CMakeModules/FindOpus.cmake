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

find_library(
		Opus_LIBRARY
		NAMES libopus opus
		HINTS ${PC_Opus_LIBDIR} ${PC_Opus_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		Opus
		DEFAULT_MSG
		Opus_INCLUDE_DIR
		Opus_LIBRARY
)

if(Opus_FOUND)
	set(Opus_INCLUDE_DIRS ${Opus_INCLUDE_DIR})
	set(Opus_LIBRARIES ${Opus_LIBRARY})
	if(NOT TARGET Opus::Opus)
		add_library(Opus::Opus UNKNOWN IMPORTED)
		set_target_properties(
				Opus::Opus
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${Opus_INCLUDE_DIR}
				IMPORTED_LOCATION ${Opus_LIBRARY}
		)
	endif()
endif()

mark_as_advanced(Opus_LIBRARY Opus_INCLUDE_DIR)
