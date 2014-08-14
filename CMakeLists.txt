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

find_package( Boost 1.55.0 COMPONENTS system filesystem thread regex program_options )

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
	"${PROJECT_SOURCE_DIR}/Internal/Utility/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Global/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/Sink.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Coordinators/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/*.cpp"
)

set( lib_lib
	${Boost_SYSTEM_LIBRARY}
	${Boost_REGEX_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	streamlog
)

add_library( ${target_lib} SHARED ${lib_src} )
target_link_libraries( ${target_lib} ${lib_lib} )

##############################################################################
# Build - Executable
##############################################################################
file( GLOB exe_src
	"${PROJECT_SOURCE_DIR}/*.cpp"
)

set( exe_lib
	${Boost_SYSTEM_LIBRARY}
	${Boost_REGEX_LIBRARY}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	streamlog
	dl
)

add_executable( ${target_exe} ${exe_src} )
target_link_libraries( ${target_exe} ${exe_lib} )

##############################################################################
# Install - Library
##############################################################################
set( Coordinator_headers "${PROJECT_SOURCE_DIR}/Internal/Coordinators/FilterCoordinator.h" )

set( Extension_headers
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Extension.h"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/ExtensionFactory.h"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/FilterExtension.h"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/FilterExtensionFactory.h"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/IOExtension.h"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/IOExtensionFactory.h"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/QueueItem.h"
)

set( Filter_headers "${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/Sink.h" )

set( Global_headers "${PROJECT_SOURCE_DIR}/Internal/Global/Logging.h" )

install( FILES ${Coordinator_headers} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent/Coordinators" )
install( FILES ${Extension_headers} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent/Extensions" )
install( FILES ${Filter_headers} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent/Extensions/Filters" )
install( FILES ${Global_headers} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Eloquent/Global" )

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
install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquent.conf" DESTINATION "${CMAKE_INSTALL_PREFIX}/etc/eloquent" )
install( FILES "${PROJECT_SOURCE_DIR}/Config/extensions.conf" DESTINATION "${CMAKE_INSTALL_PREFIX}/etc/eloquent" )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/com.evrichart.eloquentd.plist" DESTINATION "/Library/LaunchDaemons" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/com.evrichart.eloquentd.conf" DESTINATION "/etc/newsyslog.d" )

	install( CODE "execute_process( COMMAND sudo launchctl load /Library/LaunchDaemons/com.evrichart.eloquentd.plist )" )
endif()

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquentd.conf" DESTINATION "/etc/init" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquentd" DESTINATION "/etc/logrotate.d" )
endif()
