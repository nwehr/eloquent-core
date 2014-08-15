#ifndef __eloquent__IOExtensionFactory__
#define __eloquent__IOExtensionFactory__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/property_tree/ptree.hpp>

// External
#include <streamlog/streamlog.h>

// Internal
#include "Extensions/IOExtension.h"
#include "Extensions/Factories/ExtensionFactory.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// IOExtensionFactory : ExtensionFactory
	///////////////////////////////////////////////////////////////////////////////
	class IOExtensionFactory : public ExtensionFactory {
	public:
		IOExtensionFactory();
		virtual ~IOExtensionFactory();
		
		virtual IOExtension* New( const boost::property_tree::ptree::value_type& i_ConfigNode
								 , std::mutex& i_LogMutex
								 , streamlog::severity_log& i_Log
								 , std::mutex& i_QueueMutex
								 , std::condition_variable& i_QueueCV
								 , std::queue<QueueItem>& i_Queue
								 , int& i_NumWriters ) = 0;
		
	};
}

#endif /* defined(__eloquent__IOExtensionFactory__) */
