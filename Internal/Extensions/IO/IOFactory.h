#ifndef __eloquent__IOExtensionFactory__
#define __eloquent__IOExtensionFactory__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Internal
#include "Extensions/ExtensionFactory.h"
#include "Extensions/IO/IO.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// IOFactory : ExtensionFactory
	///////////////////////////////////////////////////////////////////////////////
	class IOFactory : public ExtensionFactory {
	public:
		IOFactory();
		virtual ~IOFactory();
		
		virtual IO* New( const boost::property_tree::ptree::value_type& i_ConfigNode
						, std::mutex& i_LogMutex
						, streamlog::severity_log& i_Log
						, std::mutex& i_QueueMutex
						, std::condition_variable& i_QueueCV
						, std::queue<QueueItem>& i_Queue
						, int& i_NumWriters ) = 0;
		
	};
}

#endif /* defined(__eloquent__IOExtensionFactory__) */
