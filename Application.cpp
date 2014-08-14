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
	try {
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

		if( VariableMap.count( "ext-config" ) ) {
			m_ExtConfigPath = boost::filesystem::path( VariableMap["ext-config"].as<std::string>() );
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
		
	} catch( boost::program_options::error& e ) {
		std::cout << "eloquentd: " << e.what() << std::endl;
		exit( EXIT_FAILURE );
	} catch( std::exception& e ) {
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_ERROR ) << "Application::Application() - error - " << e.what() << std::endl;
	}

	throw std::runtime_error( "Applicatoin::Init() not returning" ); 

}

Eloquent::Application::~Application(){
	for( std::tuple<void*, std::string, ExtensionFactory*>& Extension : m_Extensions ) {
		delete std::get<2>( Extension );
		dlclose( std::get<0>( Extension ) );
		
	}
	
	for( std::tuple<std::mutex*, std::condition_variable*, std::queue<QueueItem>*, int>& Queue : m_Queues ) {
		delete std::get<0>( Queue );
		delete std::get<1>( Queue );
		delete std::get<2>( Queue );
		
	}
		
}

Eloquent::ExtensionFactory* Eloquent::Application::LoadExtension( const boost::filesystem::path& i_ExtensionPath ) {
	try {
		for( std::tuple<void*, std::string, ExtensionFactory*>& Extension : m_Extensions ) {
			if( i_ExtensionPath.string() == std::get<1>( Extension ) ) {
				return std::get<2>( Extension );
			}

		}

		boost::filesystem::path ExtensionPath = i_ExtensionPath;
		
		void* Extension = dlopen( ExtensionPath.string().c_str(), RTLD_LAZY | RTLD_LOCAL );
		
		if( !Extension ) {
			std::unique_lock<std::mutex> LogLock( m_LogMutex );
			m_Log( LogSeverity::SEV_ERROR ) << "Application::LoadExtension() - error - no extension at " << ExtensionPath.string() << std::endl;
		} else {
			void* AttachSymbol = dlsym( Extension, "Attach" );
			
			if( !AttachSymbol ) {
				std::unique_lock<std::mutex> LogLock( m_LogMutex );
				m_Log( LogSeverity::SEV_ERROR ) << "Application::LoadExtension() - error - no attach symbol at " << ExtensionPath.string() << std::endl;
			} else {
				{
					std::unique_lock<std::mutex> LogLock( m_LogMutex );
					m_Log( LogSeverity::SEV_DEBUG ) << "Application::LoadExtension() - debug - attaching library " << ExtensionPath.string() << std::endl;
				}
				
				ExtensionFactory* (*AttachFunction)(void) = 0;
				
				*reinterpret_cast<void**>( &AttachFunction ) = AttachSymbol;
				
				ExtensionFactory* MyExtensionFactory = AttachFunction();
				
				m_Extensions.push_back( std::tuple<void*, std::string, ExtensionFactory*>( Extension, ExtensionPath.string(), MyExtensionFactory ) );
				
				return MyExtensionFactory;

			}

		}

	} catch( std::exception& e ) {
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_ERROR ) << "Application::LoadExtension() - error - " << e.what() << std::endl;
	}

	throw std::runtime_error( "Applicatoin::LoadExtension() not returning" ); 
	
}

int Eloquent::Application::Run() {
	try {
		if( !boost::filesystem::exists( m_ConfigPath ) ) {
			std::stringstream ErrorMessage;
			ErrorMessage << m_ConfigPath.string() << " does not exist";

			throw std::runtime_error( ErrorMessage.str() );

		}
		
		if( !boost::filesystem::exists( m_ExtConfigPath ) ) {
			std::stringstream ErrorMessage;
			ErrorMessage << m_ExtConfigPath.string() << " does not exist";

			throw std::runtime_error( ErrorMessage.str() );

		}

		std::vector<std::thread> ThreadGroup;
		
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
			
			m_Queues.push_back( std::tuple<std::mutex*, std::condition_variable*, std::queue<QueueItem>*, int>( new std::mutex(), new std::condition_variable(), new std::queue<QueueItem>(), int( 0 ) ) );
			
			for( boost::property_tree::ptree::iterator ConfigRoot_it = ConfigRoot.begin(); ConfigRoot_it != ConfigRoot.end(); ++ConfigRoot_it ) {
				boost::property_tree::ptree::value_type ConfigNode = *ConfigRoot_it;
				
				// Start Looping Through Extension Configurations (Manifest)
				for( boost::property_tree::ptree::iterator ExtConfigTree_it = ExtConfigTree.begin(); ExtConfigTree_it != ExtConfigTree.end(); ++ExtConfigTree_it ) {
					boost::property_tree::ptree ExtConfigRoot = (*ExtConfigTree_it).second;
					
					for( boost::property_tree::ptree::iterator ExtConfigRoot_it = ExtConfigRoot.begin(); ExtConfigRoot_it != ExtConfigRoot.end(); ++ ExtConfigRoot_it ) {
						boost::property_tree::ptree::value_type ExtConfigNode = *ExtConfigRoot_it;
						
						if( ConfigNode.first == ExtConfigNode.first && ConfigNode.second.get<std::string>( "name" ) == ExtConfigNode.second.get<std::string>( "name" ) ) {
							ExtensionFactory* MyExtensionFactory = LoadExtension( boost::filesystem::path( ExtConfigNode.second.get<std::string>( "path" ) ) );
							
							IOExtension* MyExtension = reinterpret_cast<IOExtensionFactory*>( MyExtensionFactory )->New( ConfigNode
																				 , m_LogMutex
																				 , m_Log
																				 , *std::get<0>( m_Queues.back() )
																				 , *std::get<1>( m_Queues.back() )
																				 , *std::get<2>( m_Queues.back() )
																				 , std::get<3>( m_Queues.back() ) );
							
							if( ConfigNode.first == "write" ) {
								++std::get<3>( m_Queues.back() );
							}
							
							ThreadGroup.push_back( std::thread( std::ref( *MyExtension ) ) );
							
						}
						
					}
					
				}
				
			}
			
		}
		
		for( std::thread& Thread: ThreadGroup )
			Thread.join();
		
		return 0;

	} catch( std::exception& e ) {
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_ERROR ) << "Application::Run() - error - " << e.what() << std::endl;
	}
	
	throw std::runtime_error( "Applicatoin::Run() not returning" ); 

}