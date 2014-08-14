##############################################################################
# Project Settings and Properties
##############################################################################
cmake_minimum_required( VERSION 2.8 )

project( eloquentd ) 

ADD_DEFINITIONS( -std=c++11 )

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
   
include_directories( "${PROJECT_SOURCE_DIR}/Internal" )
include_directories( "${PROJECT_SOURCE_DIR}/External" )

include_directories( "/usr/include" )
include_directories( "/usr/local/include" )
include_directories( "/opt/local/include" )

##############################################################################
# Library Search Paths
##############################################################################
link_directories( "/usr/lib" )
link_directories( "/usr/local/lib" )
link_directories( "/opt/local/lib" )

##############################################################################
# Build - Executable
##############################################################################
file( GLOB common_src
	"${PROJECT_SOURCE_DIR}/Internal/Utility/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Global/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/Sink.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Coordinators/*.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/*.cpp"
)

set( common_lib 
	${Boost_SYSTEM_LIBRARY}
	${Boost_REGEX_LIBRARY}
	streamlog
)

file( GLOB eloquent_src
	"${PROJECT_SOURCE_DIR}/Internal/*.cpp"
	"${PROJECT_SOURCE_DIR}/*.cpp"
)

set( eloquent_lib
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	dl
)

add_executable( eloquentd ${common_src} ${eloquent_src} )
target_link_libraries( eloquentd ${common_lib} ${eloquent_lib} )

##############################################################################
# Build - FileReader
##############################################################################
file( GLOB FileReader_src
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/FileReader.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/FileReaderFactory.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/FileReaderAttach.cpp"
)

set( FileReader_lib
	${Boost_FILESYSTEM_LIBRARY}
)

add_library( FileReader SHARED ${common_src} ${FileReader_src} )
target_link_libraries( FileReader ${common_lib} ${FileReader_lib} )

##############################################################################
# Build - RestReader
##############################################################################
file( GLOB RestReader_src
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/RestReader.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/RestReaderFactory.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/RestReaderAttach.cpp"
)

add_library( RestReader SHARED ${common_src} ${RestReader_src} )
target_link_libraries( RestReader ${common_lib} PocoFoundation PocoNet PocoUtil )

##############################################################################
# Build - DatagramReader
##############################################################################
file( GLOB DatagramReader_src
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/DatagramReader.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/DatagramReaderFactory.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Readers/DatagramReaderAttach.cpp"
)

add_library( DatagramReader SHARED ${common_src} ${DatagramReader_src} )
target_link_libraries( DatagramReader ${common_lib} )

##############################################################################
# Build - ConsoleWriter
##############################################################################
file( GLOB ConsoleWriter_src
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/ConsoleWriter.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/ConsoleWriterFactory.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/ConsoleWriterAttach.cpp"
)

add_library( ConsoleWriter SHARED ${common_src} ${ConsoleWriter_src} )
target_link_libraries( ConsoleWriter ${common_lib} )

##############################################################################
# Build - FileWriter
##############################################################################
file( GLOB FileWriter_src
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/FileWriter.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/FileWriterFactory.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/FileWriterAttach.cpp"
)

set( FileWriter_lib
	${Boost_FILESYSTEM_LIBRARY}
)

add_library( FileWriter SHARED ${common_src} ${FileWriter_src} )
target_link_libraries( FileWriter ${common_lib} ${FileWriter_lib} )

##############################################################################
# Build - RestWriter
##############################################################################
file( GLOB RestWriter_src
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/RestWriter.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/RestWriterFactory.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/RestWriterAttach.cpp"
)

add_library( RestWriter SHARED ${common_src} ${RestWriter_src} )
target_link_libraries( RestWriter ${common_lib} PocoFoundation PocoNet )

##############################################################################
# Build - DatagramWriter
##############################################################################
file( GLOB DatagramWriter_src
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/DatagramWriter.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/DatagramWriterFactory.cpp"
	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/DatagramWriterAttach.cpp"
)

add_library( DatagramWriter SHARED ${common_src} ${DatagramWriter_src} )
target_link_libraries( DatagramWriter ${common_lib} PocoFoundation PocoNet )

# file( GLOB RegexFilter_src
#	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/RegexFilter.cpp"
#	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/RegexFilterFactory.cpp"
#	"${PROJECT_SOURCE_DIR}/Internal/Extensions/Filters/RegexFilterAttach.cpp"
# )

# add_library( RegexFilter SHARED ${common_src} ${RegexFilter_src} )
# target_link_libraries( RegexFilter boost_system boost_regex streamlog )

# add_library( MongoWriter SHARED "${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/MKFTMongoWriter.cpp" )
# target_link_libraries( MongoWriter JsonBox boost_filesystem boost_regex boost_thread boost_system mongoclient )

# add_library( NullWriter SHARED "${PROJECT_SOURCE_DIR}/Internal/Extensions/Writers/MKFTNullWriter.cpp" )
# target_link_libraries( NullWriter JsonBox boost_system boost_thread )

##############################################################################
# Install - Executable
##############################################################################
install( PROGRAMS "${PROJECT_SOURCE_DIR}/build/eloquentd" DESTINATION /usr/sbin )

##############################################################################
# Install - Libraries
##############################################################################
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set_target_properties( FileReader PROPERTIES MACOSX_RPATH 1 )
	set_target_properties( RestReader PROPERTIES MACOSX_RPATH 1 )
	set_target_properties( DatagramReader PROPERTIES MACOSX_RPATH 1 )
	set_target_properties( FileWriter PROPERTIES MACOSX_RPATH 1 )
	set_target_properties( RestWriter PROPERTIES MACOSX_RPATH 1 )
	set_target_properties( ConsoleWriter PROPERTIES MACOSX_RPATH 1 )
	set_target_properties( DatagramWriter PROPERTIES MACOSX_RPATH 1 )
endif()

install( TARGETS FileReader LIBRARY DESTINATION "${ETC_PREFIX}/etc/eloquent/extensions" )
install( TARGETS RestReader LIBRARY DESTINATION "${ETC_PREFIX}/etc/eloquent/extensions" )
install( TARGETS DatagramReader LIBRARY DESTINATION "${ETC_PREFIX}/etc/eloquent/extensions" )
install( TARGETS FileWriter LIBRARY DESTINATION "${ETC_PREFIX}/etc/eloquent/extensions" )
install( TARGETS RestWriter DESTINATION "${ETC_PREFIX}/etc/eloquent/extensions" )
install( TARGETS ConsoleWriter LIBRARY DESTINATION "${ETC_PREFIX}/etc/eloquent/extensions" )
install( TARGETS DatagramWriter LIBRARY DESTINATION "${ETC_PREFIX}/etc/eloquent/extensions" )

##############################################################################
# Install - Configurations
##############################################################################
set( ETC_PREFIX "" )

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set( ETC_PREFIX "/private" )
endif()

if( NOT EXISTS "${ETC_PREFIX}/etc/eloquent/eloquent.conf" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquent.conf" DESTINATION "${ETC_PREFIX}/etc/eloquent" )
endif()

if( NOT EXISTS "${ETC_PREFIX}/etc/eloquent/extensions.conf" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/extensions.conf" DESTINATION "${ETC_PREFIX}/etc/eloquent" )
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	install( FILES "${PROJECT_SOURCE_DIR}/Config/com.evrichart.eloquentd.plist" DESTINATION "/Library/LaunchDaemons" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/com.evrichart.eloquentd.conf" DESTINATION "/etc/newsyslog.d" )

	install( CODE "execute_process( COMMAND sudo launchctl load /Library/LaunchDaemons/com.evrichart.eloquentd.plist )" )
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquentd.conf" DESTINATION "/etc/init" )
	install( FILES "${PROJECT_SOURCE_DIR}/Config/eloquentd" DESTINATION "/etc/logrotate.d" )
endif()
