#ifndef __eloquentd__ExtensionManager__
#define __eloquentd__ExtensionManager__

//
//  ExtensionManager.h
//  eloquentd
//
//  Created by Nathan Wehr on 8/20/14.
//  Copyright (c) 2014 EvriChart, Inc. All rights reserved.
//

// C++
#include <deque>
#include <tuple>
#include <string>

// Boost
#include <boost/filesystem.hpp>

// External
#include <streamlog/streamlog.h>

// Internal
#include "ExtensionFactory.h"

namespace Eloquent {
	class ExtensionManager {
	public:
		static ExtensionManager* m_Instance;
		
		explicit ExtensionManager( std::mutex&, streamlog::severity_log& );
		~ExtensionManager();
		
		static ExtensionManager& Instance( std::mutex&, streamlog::severity_log& );
		
		ExtensionFactory* LoadExtension( const boost::filesystem::path& );
		
	private:
		std::deque<std::tuple<void*, std::string, ExtensionFactory*>> m_Extensions;
		
		// Logging
		std::mutex& m_LogMutex;
		streamlog::severity_log& m_Log;
		
	};
}

#endif /* defined(__eloquentd__ExtensionManager__) */
