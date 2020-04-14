#  VorbisFile_FOUND - System has VorbisFile
#  VorbisFile_INCLUDE_DIRS - The VorbisFile include directories
#  VorbisFile_LIBRARIES - The libraries needed to use VorbisFile

find_package(PkgConfig)
pkg_check_modules(PC_VorbisFile QUIET vorbisfile)

find_path(
		VorbisFile_INCLUDE_DIR
		vorbis/vorbisfile.h
		HINTS ${PC_VorbisFile_INCLUDEDIR} ${PC_VorbisFile_INCLUDE_DIRS}
)

find_library(
		VorbisFile_LIBRARY
		NAMES libvorbisfile vorbisfile
		HINTS ${PC_VorbisFile_LIBDIR} ${PC_VorbisFile_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		VorbisFile
		DEFAULT_MSG
		VorbisFile_LIBRARY
		VorbisFile_INCLUDE_DIR
)

if(VorbisFile_FOUND)
	set(VorbisFile_LIBRARIES ${VorbisFile_LIBRARY})
	set(VorbisFile_INCLUDE_DIRS ${VorbisFile_INCLUDE_DIR})

	if(NOT TARGET VorbisFile::VorbisFile)
		add_library(VorbisFile::VorbisFile UNKNOWN IMPORTED)
		set_target_properties(
				VorbisFile::VorbisFile
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${VorbisFile_INCLUDE_DIR}
				IMPORTED_LOCATION ${VorbisFile_LIBRARY}
		)
	endif()
endif()

mark_as_advanced(VorbisFile_LIBRARY VorbisFile_INCLUDE_DIR)
