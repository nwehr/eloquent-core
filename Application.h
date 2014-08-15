#ifndef _Application_h
#define _Application_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <deque>
#include <queue>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

// Boost
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>

// External
#include <streamlog/streamlog.h>

// Internal
#include "Extensions/Extension.h"
#include "Extensions/Factories/ExtensionFactory.h"
#include "QueueItem.h"

namespace Eloquent {
	static const std::string APP_NAME		= "eloquentd";
	static const std::string APP_VERSION	= "1.0";
	static const std::string APP_RELEASE	= "BETA";
	
	static const std::string CONFIG_PATH		= "/usr/local/etc/eloquent/eloquent.conf";
	static const std::string EXT_CONFIG_PATH 	= "/usr/local/etc/eloquent/extensions.conf";
	static const std::string LOG_PATH			= "/var/log/eloquent.log";
	
	///////////////////////////////////////////////////////////////////////////////
	// Application
	///////////////////////////////////////////////////////////////////////////////
	class Application {
		Application();
		Application( const Application& );
		void operator=( const Application& );
		
	public:
		~Application();
		
		Application& Init( int argc, const char* argv[] );
		
		ExtensionFactory* LoadExtension( const boost::filesystem::path& );
		int Run();

		static Application& Instance();
		
	private:
		// Arguement Options
		int 					m_LogLevel;
		boost::filesystem::path m_LogPath;
		boost::filesystem::path m_ConfigPath;
		boost::filesystem::path m_ExtConfigPath;
		
		// Logging
		std::mutex 				m_LogMutex;
		streamlog::severity_log m_Log;
		
		// Extensions
		std::deque<std::tuple<void*, std::string, ExtensionFactory*>> m_Extensions;
		
		// Queues
		std::deque<std::tuple<std::mutex*, std::condition_variable*, std::queue<QueueItem>*, int>> m_Queues;

	public:
		// Instance Pointer
		static Application* m_Application;
		
	};

}

#endif // _Application_h
