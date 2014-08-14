//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C
#include <dlfcn.h>

// C++
#include <iostream>
#include <fstream>
#include <cerrno>
#include <thread>

// Boost
#include <boost/property_tree/info_parser.hpp>

#include <boost/foreach.hpp>
#include <boost/program_options.hpp>

// Internal
#include "Extensions/IOExtension.h"
#include "Extensions/IOExtensionFactory.h"

#include "Extensions/FilterExtension.h"
#include "Extensions/FilterExtensionFactory.h"

#include "Global/Logging.h"

#include "Application.h"


///////////////////////////////////////////////////////////////////////////////
// Application
///////////////////////////////////////////////////////////////////////////////
Eloquent::Application::Application()
: m_LogLevel( LogSeverity::SEV_WARNING )
, m_LogPath( LOG_PATH )
, m_ConfigPath( CONFIG_PATH )
, m_ExtConfigPath( EXT_CONFIG_PATH )
, m_Log()
{}

Eloquent::Application* Eloquent::Application::m_Application;

Eloquent::Application& Eloquent::Application::Instance() {
	if( !m_Application )
		m_Application = new Eloquent::Application();

	return *m_Application;
}

Eloquent::Application& Eloquent::Application::Init( int argc, const char* argv[] ) {
	boost::program_options::options_description OptionsDescription( "Options" );
	
	OptionsDescription.add_options()
		("help,h"			, "Displays options")
		("version,v"		, "Displays version")
		("log,l"			, boost::program_options::value<std::string>(), "Set path to log file")
		("log-level,s"		, boost::program_options::value<std::string>(), "Set log severity level\n[ trace | debug | info | warning | error | fatal ]")
		("config,c"			, boost::program_options::value<std::string>(), "Set path to eloquent config file")
		("ext-config,e"		, boost::program_options::value<std::string>(), "Set path to extensions config file")
		;

	boost::program_options::variables_map VariableMap;

	try {
		boost::program_options::store( boost::program_options::parse_command_line( argc, argv, OptionsDescription ), VariableMap );
		
		boost::program_options::notify( VariableMap );
		
		if ( VariableMap.count( "help" ) ) {
			std::cout << OptionsDescription << std::endl;
			exit( EXIT_SUCCESS );
		}
		
		if ( VariableMap.count( "version" ) ) {
			std::cout << APP_NAME << " version " << APP_VERSION << " " << APP_RELEASE << std::endl;
			exit( EXIT_SUCCESS );
		}

		if ( VariableMap.count( "log-level" ) ) {
			if( VariableMap["log-level"].as<std::string>() == "trace" ) {
				m_LogLevel = LogSeverity::SEV_TRACE;
			} else if( VariableMap["log-level"].as<std::string>() == "debug" ) {
				m_LogLevel = LogSeverity::SEV_DEBUG;
			} else if( VariableMap["log-level"].as<std::string>() == "info" ) {
				m_LogLevel = LogSeverity::SEV_INFO;
			} else if( VariableMap["log-level"].as<std::string>() == "warning" ) {
				m_LogLevel = LogSeverity::SEV_WARNING;
			} else if( VariableMap["log-level"].as<std::string>() == "error" ) {
				m_LogLevel = LogSeverity::SEV_ERROR;
			} else if( VariableMap["log-level"].as<std::string>() == "fatal" ) {
				m_LogLevel = LogSeverity::SEV_FATAL;
			} else {
				std::cout << "eloquentd: '" << VariableMap["log-level"].as<std::string>() << "' is not a valid log-level option." << std::endl << OptionsDescription << std::endl;
				exit( EXIT_FAILURE );
			}
			
		}
		
		if( VariableMap.count( "log" ) ) {
			m_LogPath = boost::filesystem::path( VariableMap["log"].as<std::string>() );
		}
		
		if( VariableMap.count( "config" ) ) {
			m_ConfigPath = boost::filesystem::path( VariableMap["config"].as<std::string>() );
		}

		if( VariableMap.count( "extensions" ) ) {
			m_ExtConfigPath = boost::filesystem::path( VariableMap["ext-config"].as<std::string>() );
		}


		
	} catch( boost::program_options::error& e ) {
		std::cout << "eloquentd: " << e.what() << std::endl;
		std::cout << OptionsDescription << std::endl;
		exit( EXIT_FAILURE );
	}

	m_Log.set_severity( m_LogLevel );

	std::string m_LogPathString = m_LogPath.string();

	m_Log.add_stream( &std::cout );
	m_Log.add_stream( new std::ofstream( m_LogPathString.c_str(), std::ios_base::out ), true );

	{
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_INFO ) << "Application::Application() - info - setting up application" << std::endl;
	}

	return *this;
}

Eloquent::Application::~Application(){
	for( std::deque<std::tuple<void*, std::string, boost::shared_ptr<ExtensionFactory>>>::iterator it = m_Extensions.begin(); it != m_Extensions.end(); ++it )
		dlclose( std::get<0>( *it ) );
}

boost::shared_ptr<Eloquent::ExtensionFactory> Eloquent::Application::LoadExtension( const boost::filesystem::path& i_ExtensionPath ) {
	for( std::deque<std::tuple<void*, std::string, boost::shared_ptr<ExtensionFactory>>>::iterator it = m_Extensions.begin(); it != m_Extensions.end(); ++it ) {
		if( i_ExtensionPath.string() == std::get<1>( *it ) ) {
			return std::get<2>( *it );
		}
	}
	
	boost::filesystem::path ExtensionPath = i_ExtensionPath;
	
	void* Extension = dlopen( ExtensionPath.string().c_str(), RTLD_LAZY | RTLD_LOCAL );
	
	if( !Extension ) {
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_ERROR ) << "Application::LoadExtension() - error - no extension at " << ExtensionPath.string() << std::endl;
	}

	void* AttachSymbol = dlsym( Extension, "Attach" );
	
	if( !AttachSymbol ) {
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_ERROR ) << "Application::LoadExtension() - error - not attach symbole at " << ExtensionPath.string() << std::endl;
	}
	
	{
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_DEBUG ) << "Application::LoadExtension() - debug - attaching library " << ExtensionPath.string() << std::endl;
	}
	
	boost::shared_ptr<ExtensionFactory> (*AttachFunction)(void) = 0;
	
	*reinterpret_cast<void**>( &AttachFunction ) = AttachSymbol;
	
	boost::shared_ptr<ExtensionFactory> MyExtensionFactory = AttachFunction();
	
	m_Extensions.push_back( std::tuple<void*, std::string, boost::shared_ptr<ExtensionFactory>>( Extension, ExtensionPath.string(), MyExtensionFactory ) );
	
	return MyExtensionFactory;
	
}

int Eloquent::Application::Run() {
	std::vector<std::thread> ThreadGroup;

	if( !boost::filesystem::exists( m_ConfigPath ) ) {
		throw std::exception();
	}
	
	if( !boost::filesystem::exists( m_ConfigPath ) ) {
		throw std::exception();
	}
	
	std::ifstream ConfigStream( m_ConfigPath.string().c_str(), std::ifstream::in );
	std::ifstream ExtConfigStream( m_ExtConfigPath.string().c_str(), std::ifstream::in );
	
	// Daemon Configurations
	boost::property_tree::ptree ConfigTree;
	boost::property_tree::info_parser::read_info( ConfigStream, ConfigTree );
	
	// Extension Configurations (Manifest)
	boost::property_tree::ptree ExtConfigTree;
	boost::property_tree::info_parser::read_info( ExtConfigStream, ExtConfigTree );
	
	// Start Looping Through Daemon Configurations
	for( boost::property_tree::ptree::iterator ConfigTree_it = ConfigTree.begin(); ConfigTree_it != ConfigTree.end(); ++ConfigTree_it  ) {
		boost::property_tree::ptree ConfigRoot = (*ConfigTree_it).second;
		
		m_Queue.push( std::tuple<std::mutex*, std::condition_variable*, std::queue<QueueItem>*, int>( new std::mutex(), new std::condition_variable(), new std::queue<QueueItem>(), int( 0 ) ) );
		
		for( boost::property_tree::ptree::iterator ConfigRoot_it = ConfigRoot.begin(); ConfigRoot_it != ConfigRoot.end(); ++ConfigRoot_it ) {
			boost::property_tree::ptree::value_type ConfigNode = *ConfigRoot_it;
			
			// Start Looping Through Extension Configurations (Manifest)
			for( boost::property_tree::ptree::iterator ExtConfigTree_it = ExtConfigTree.begin(); ExtConfigTree_it != ExtConfigTree.end(); ++ExtConfigTree_it ) {
				boost::property_tree::ptree ExtConfigRoot = (*ExtConfigTree_it).second;
				
				for( boost::property_tree::ptree::iterator ExtConfigRoot_it = ExtConfigRoot.begin(); ExtConfigRoot_it != ExtConfigRoot.end(); ++ ExtConfigRoot_it ) {
					boost::property_tree::ptree::value_type ExtConfigNode = *ExtConfigRoot_it;
					
					if( ConfigNode.first == ExtConfigNode.first && ConfigNode.second.get<std::string>( "name" ) == ExtConfigNode.second.get<std::string>( "name" ) ) {
						boost::shared_ptr<ExtensionFactory> MyExtensionFactory = LoadExtension( boost::filesystem::path( ExtConfigNode.second.get<std::string>( "path" ) ) );
						IOExtensionFactory* MyIOExtensionFactory = reinterpret_cast<IOExtensionFactory*>( &*MyExtensionFactory );
						
						IOExtension* MyExtension = MyIOExtensionFactory->New( ConfigNode
																			 , m_LogMutex
																			 , m_Log
																			 , *std::get<0>( m_Queue.back() )
																			 , *std::get<1>( m_Queue.back() )
																			 , *std::get<2>( m_Queue.back() )
																			 , std::get<3>( m_Queue.back() ) );
						
						if ( ConfigNode.first == "write" ) {
							++std::get<3>( m_Queue.back() );
						}
						
						ThreadGroup.push_back( std::thread( std::ref( *MyExtension ) ) );
						
					}
					
				}
				
			}
			
		}
		
	}
	
	for( auto& thread: ThreadGroup )
		thread.join();
	
	return 0;
	
}