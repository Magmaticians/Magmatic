#  OpusFile_FOUND - System has OpusFile
#  OpusFile_INCLUDE_DIRS - The OpusFile include directories
#  OpusFile_LIBRARIES - The libraries needed to use OpusFile

find_package(PkgConfig QUIET)
pkg_check_modules(PC_OpusFile QUIET opusfile)

find_package(Opus REQUIRED)

find_path(
		OpusFile_INCLUDE_DIR
		opus/opusfile.h
		HINTS ${PC_OpusFile_INCLUDEDIR} ${PC_OpusFile_INCLUDE_DIRS}
)

find_library(
		OpusFile_LIBRARY
		NAMES libopusfile opusfile
		HINTS ${PC_OpusFile_LIBDIR} ${PC_OpusFile_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		OpusFile
		DEFAULT_MSG
		OpusFile_LIBRARY
		OpusFile_INCLUDE_DIR
		Opus_INCLUDE_DIR
)

if(OpusFile_FOUND)
	set(OpusFile_LIBRARIES ${OpusFile_LIBRARY})
	set(OpusFile_INCLUDE_DIRS ${OpusFile_INCLUDE_DIR})
	if(NOT TARGET OpusFile::OpusFile)
		add_library(OpusFile::OpusFile UNKNOWN IMPORTED)
		set_target_properties(
				OpusFile::OpusFile
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${OpusFile_INCLUDE_DIR}
				IMPORTED_LOCATION ${OpusFile_LIBRARY}
		)
		target_link_libraries(OpusFile::OpusFile INTERFACE Opus::Opus)
	endif()
endif()

mark_as_advanced(OpusFile_LIBRARY OpusFile_INCLUDE_DIR)
