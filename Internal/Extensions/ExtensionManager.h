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
#include <thread>
#include <mutex>

// Boost
#include <boost/filesystem.hpp>

// External
#include <streamlog/streamlog.h>

// Internal
#include "ExtensionFactory.h"

namespace Eloquent {
	class ExtensionManager {
	public:
		ExtensionManager();
		~ExtensionManager();
		
		static ExtensionManager* m_Instance;
		static ExtensionManager& Instance();
		
		ExtensionFactory* LoadExtension( const boost::filesystem::path& );
		
	private:
		std::deque<std::tuple<void*, std::string, ExtensionFactory*>> m_Extensions;
		
	};
}

#endif /* defined(__eloquentd__ExtensionManager__) */
