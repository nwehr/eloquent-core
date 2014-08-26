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
#include "Extensions/ExtensionFactory.h"

#include "Extensions/Filters/Filter.h"
#include "Extensions/Filters/FilterFactory.h"

#include "Extensions/ExtensionManager.h"

#include "QueueItem.h"

namespace Eloquent {
	static const std::string APP_NAME		= "eloquentd";
	static const std::string APP_VERSION	= "1.0.0_0";
	static const std::string APP_RELEASE	= "RELEASE";
	
	static const std::string CONFIG_PATH	= "/usr/local/etc/eloquent/eloquent.conf";
	static const std::string LOG_PATH		= "/var/log/eloquent.log";
	
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
		
		int Run();

		static Application& Instance();
		
	private:
		// Arguement Options
		int m_LogLevel;
		boost::filesystem::path m_LogPath;
		boost::filesystem::path m_ConfigPath;
		
		// Logging
		std::mutex m_LogMutex;
		streamlog::severity_log m_Log;
		
		// Queues and Filters
		std::deque<std::tuple<std::mutex*, std::condition_variable*, std::queue<QueueItem>*, int>> m_Queues;

	public:
		// Instance Pointer
		static Application* m_Application;
		
	};

}

#endif // _Application_h
