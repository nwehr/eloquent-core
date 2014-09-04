#ifndef _Application_h
#define _Application_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C
#include <syslog.h>

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
	
	///////////////////////////////////////////////////////////////////////////////
	// Application
	///////////////////////////////////////////////////////////////////////////////
	class Application {
		typedef std::tuple<std::mutex*, std::condition_variable*, std::queue<QueueItem>*, unsigned int> Queue_t;
		
		Application();
		Application( const Application& );
		void operator=( const Application& );
		
	public:
		~Application();
		
		Application& Init( int argc, const char* argv[] );
		
		static Application* m_Application;
		static Application& Instance();
		
		int Run();
		
	private:
		// Arguement Options
		int m_LogLevel;
		boost::filesystem::path m_ConfigPath;
		
		// Queues and Filters
		std::deque<Queue_t> m_Queues;
		
	};

}

#endif // _Application_h
