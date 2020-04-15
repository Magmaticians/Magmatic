#  Ogg_FOUND - System has Ogg
#  Ogg_INCLUDE_DIRS - The Opus include directories
#  Ogg_LIBRARIES - The libraries needed to use Opus

find_package(PkgConfig QUIET)
pkg_check_modules(PC_Opus QUIET ogg)

find_path(
		Ogg_INCLUDE_DIR
		ogg/ogg.h
		HINTS ${PC_Ogg_INCLUDEDIR} ${PC_Ogg_INCLUDE_DIRS}
)

find_library(
		Ogg_LIBRARY
		NAMES libogg ogg
		HINTS ${PC_Ogg_LIBDIR} ${PC_Ogg_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
		Ogg
		DEFAULT_MSG
		Ogg_INCLUDE_DIR
		Ogg_LIBRARY
)

if(Ogg_FOUND)
	set(Ogg_INCLUDE_DIRS ${Ogg_INCLUDE_DIR})
	set(Ogg_LIBRARIES ${Ogg_LIBRARY})
	if(NOT TARGET Ogg::Ogg)
		add_library(Ogg::Ogg UNKNOWN IMPORTED)
		set_target_properties(
				Ogg::Ogg
				PROPERTIES
				INTERFACE_INCLUDE_DIRECTORIES ${Ogg_INCLUDE_DIR}
				IMPORTED_LOCATION ${Ogg_LIBRARY}
		)
	endif()
endif()

mark_as_advanced(Ogg_LIBRARY Ogg_INCLUDE_DIR)
