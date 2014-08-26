#ifndef __eloquent__FilterExtensionFactory__
#define __eloquent__FilterExtensionFactory__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Internal
#include "Extensions/ExtensionFactory.h"
#include "Extensions/Filters/Filter.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// FilterFactory : ExtensionFactory
	///////////////////////////////////////////////////////////////////////////////
	class FilterFactory : public ExtensionFactory {
	public:
		FilterFactory();
		virtual ~FilterFactory();
		
		virtual Filter* New( const boost::property_tree::ptree::value_type& i_ConfigNode
									 , std::mutex& i_LogMutex
									 , streamlog::severity_log& i_Log ) = 0;
		
	};
	
}

#endif /* defined(__eloquent__FilterExtensionFactory__) */
