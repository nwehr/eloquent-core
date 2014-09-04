//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <iostream>
#include <fstream>
#include <cerrno>
#include <thread>

// Boost
#include <boost/property_tree/info_parser.hpp>
#include <boost/program_options.hpp>

// Internal
#include "Extensions/IO/IO.h"
#include "Extensions/IO/IOFactory.h"

#include "Extensions/Filters/Filter.h"
#include "Extensions/Filters/FilterFactory.h"

#include "Application.h"

///////////////////////////////////////////////////////////////////////////////
// Application
///////////////////////////////////////////////////////////////////////////////
Eloquent::Application::Application()
: m_LogLevel( LOG_WARNING )
, m_ConfigPath( CONFIG_PATH )
{
	openlog( NULL, LOG_CONS | LOG_NDELAY | LOG_PID, LOG_LOCAL7 );
}

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
		("log-level,s"		, boost::program_options::value<std::string>(), "Set log severity level\n[ debug | info | warning | error | fatal ]")
		("config,c"			, boost::program_options::value<std::string>(), "Set path to eloquent config file")
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
			if( VariableMap["log-level"].as<std::string>() == "debug" ) {
				m_LogLevel = LOG_DEBUG;
			} else if( VariableMap["log-level"].as<std::string>() == "info" ) {
				m_LogLevel = LOG_INFO;
			} else if( VariableMap["log-level"].as<std::string>() == "warning" ) {
				m_LogLevel = LOG_WARNING;
			} else if( VariableMap["log-level"].as<std::string>() == "error" ) {
				m_LogLevel = LOG_ERR;
			} else if( VariableMap["log-level"].as<std::string>() == "fatal" ) {
				m_LogLevel = LOG_CRIT;
			} else {
				std::cout
				<< "eloquentd: '"
				<< VariableMap["log-level"].as<std::string>()
				<< "' is not a valid log-level option."
				<< std::endl
				<< OptionsDescription
				<< std::endl;
				
				exit( EXIT_FAILURE );
				
			}
			
			setlogmask( LOG_UPTO( m_LogLevel ) );
			
		}
		
		if( VariableMap.count( "config" ) ) {
			m_ConfigPath = boost::filesystem::path( VariableMap["config"].as<std::string>() );
		}

	} catch( boost::program_options::error& e ) {
		std::cout << "eloquentd: " << e.what() << std::endl;
		exit( EXIT_FAILURE );
	} catch( std::exception& e ) {
		syslog( LOG_ERR, "%s #Application::Init() #Error", e.what() );
	}

	return *this;

}

Eloquent::Application::~Application(){
	for( Queue_t& Queue : m_Queues ) {
		delete std::get<0>( Queue );
		delete std::get<1>( Queue );
		delete std::get<2>( Queue );
		
	}
	
}

int Eloquent::Application::Run() {
	try {
		// Does our config file exist? If not, we're dead in the water
		if( !boost::filesystem::exists( m_ConfigPath ) ) {
			std::stringstream ErrorMessage;
			ErrorMessage << m_ConfigPath.string() << " does not exist";

			throw std::runtime_error( ErrorMessage.str() );

		}
		
		std::vector<std::thread> ThreadGroup;
		
		// Open up the config file
		std::ifstream ConfigStream( m_ConfigPath.string().c_str(), std::ifstream::in );
		
		// Move the contents of the config file into a property tree
		boost::property_tree::ptree ConfigTree;
		boost::property_tree::info_parser::read_info( ConfigStream, ConfigTree );
		
		// Close the config file since its not needed anymore
		if( ConfigStream.is_open() ) {
			ConfigStream.close();
		}
		
		// Start iterating over config property tree
		for( boost::property_tree::ptree::iterator ConfigTree_it = ConfigTree.begin(); ConfigTree_it != ConfigTree.end(); ++ConfigTree_it  ) {
			boost::property_tree::ptree ConfigRoot = (*ConfigTree_it).second;
			
			// Set up a new queue and its locking mechanisms
			m_Queues.push_back( Queue_t( new std::mutex(), new std::condition_variable(), new std::queue<QueueItem>(), (unsigned int)( 0 ) ) );
			
			for( boost::property_tree::ptree::iterator ConfigRoot_it = ConfigRoot.begin(); ConfigRoot_it != ConfigRoot.end(); ++ConfigRoot_it ) {
				boost::property_tree::ptree::value_type ConfigNode = *ConfigRoot_it;
				
				// Get the path for our extension from the config file
				boost::filesystem::path ExtensionPath( ConfigNode.second.get<std::string>( "load" ) );
				
				// Have the extension manager give us a factory object for given extension path
				IOFactory* MyFactory = reinterpret_cast<IOFactory*>( ExtensionManager::Instance().LoadExtension( ExtensionPath ) );
				
				if( !MyFactory ) {
					throw std::runtime_error( "null extension factory" );
				}
				
				// Construct a new extension
				IO* MyExtension = MyFactory->New( ConfigNode, *std::get<0>( m_Queues.back() ), *std::get<1>( m_Queues.back() ), *std::get<2>( m_Queues.back() ), std::get<3>( m_Queues.back() ) );
				
				if( !MyExtension ) {
					throw std::runtime_error( "null extension" );
				}
				
				// Is our new extension a writer? If so, we need to increment the number of writers that access the queue.
				// This used to determine when its ok to remove an item.
				if( ConfigNode.first == "write" ) {
					++std::get<3>( m_Queues.back() );
				}
				
				// Spool up our extension in a new thread
				ThreadGroup.push_back( std::thread( std::ref( *MyExtension ) ) );
				
			}
			
		}
		
		// Wait here for all eternity...
		for( std::thread& Thread: ThreadGroup )
			Thread.join();
		
		return EXIT_SUCCESS;

	} catch( std::exception& e ) {
		syslog( LOG_ERR, "%s #Application::Run() #Error", e.what() );
	}
	
	return EXIT_FAILURE;

}