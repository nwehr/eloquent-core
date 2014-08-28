##############################################################################
# Project Settings and Properties
##############################################################################
cmake_minimum_required( VERSION 2.8 )

project( eloquentd )

ADD_DEFINITIONS( -std=c++11 )

set( target_exe eloquentd )
set( target_lib Eloquent )

##############################################################################
# Header Search Paths
##############################################################################
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME	 OFF)

find_package( Boost 1.55.0 COMPONENTS system filesystem regex program_options )

if(Boost_FOUND)
	include_directories( ${Boost_INCLUDE_DIRS} )
endif()

include_directories( "${PROJECT_SOURCE_DIR}" )
include_directories( "${PROJECT_SOURCE_DIR}/Internal" )

include_directories( "/opt/local/include" )
include_directories( "/usr/include" )
include_directories( "/usr/local/include" )

##############################################################################
# Library Search Paths
##############################################################################
link_directories( "/opt/local/lib" )
link_directories( "/usr/lib" )
link_directories( "/usr/local/lib" )

##############################################################################
# Build - Library
##############################################################################
file( GLOB lib_src
	"${PROJECT_SOURCE_DIR}/Internal/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/IO/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/*.cpp"
)

set( lib_lib
	${Boost_SYSTEM_LIBRARY}
	${Boost_REGEX_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	streamlog
	dl
)

add_library( ${target_lib} SHARED ${lib_src} )
target_link_libraries( ${target_lib} ${lib_lib} )

##############################################################################
# Build - Executable
##############################################################################
file( GLOB exe_src "${PROJECT_SOURCE_DIR}/*.cpp" )

set( exe_lib
	${Boost_SYSTEM_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	dl
	Eloquent
)

add_executable( ${target_exe} ${exe_src} )
target_link_libraries( ${target_exe} ${exe_lib} )

##############################################################################
# Install - Library
##############################################################################
file( GLOB main_headers				"${PROJECT_SOURCE_DIR}/Internal/*.h" )
file( GLOB extension_headers		"${PROJECT_SOURCE_DIR}/Internal/Extensions/*.h" )
file( GLOB filter_headers			"${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/*.h" )
file( GLOB io_headers				"${PROJECT_SOURCE_DIR}/Internal/Extensions/IO/*.h" )

install( FILES ${main_headers}		DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent" )
install( FILES ${extension_headers} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent/Extensions" )
install( FILES ${filter_headers}	DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent/Extensions/Filters" )
install( FILES ${io_headers}		DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent/Extensions/IO" )

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set_target_properties( ${target_lib} PROPERTIES MACOSX_RPATH 1 )
endif()

install( TARGETS ${target_lib} LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" )

##############################################################################
# Install - Executable
##############################################################################
install( TARGETS ${target_exe} DESTINATION ${CMAKE_INSTALL_PREFIX}/sbin )

##############################################################################
# Install - Configurations
##############################################################################
if( NOT EXISTS "${CMAKE_INSTALL_PREFIX}/etc/eloquent/eloquent.conf" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquent.conf" DESTINATION "${CMAKE_INSTALL_PREFIX}/etc/eloquent" )
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
	if( NOT EXISTS "/Library/LaunchDaemons/com.evrichart.eloquentd.plist" )
		install( FILES "${PROJECT_SOURCE_DIR}/Config/com.evrichart.eloquentd.plist" DESTINATION "/Library/LaunchDaemons" )
		install( CODE "execute_process( COMMAND sudo launchctl load /Library/LaunchDaemons/com.evrichart.eloquentd.plist )" )
	endif()
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquentd.conf" DESTINATION "/etc/init" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquentd" DESTINATION "/etc/logrotate.d" )
endif()
