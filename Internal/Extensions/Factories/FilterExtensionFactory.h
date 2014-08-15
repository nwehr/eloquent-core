#ifndef __eloquent__FilterExtensionFactory__
#define __eloquent__FilterExtensionFactory__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/property_tree/ptree.hpp>

// External
#include <streamlog/streamlog.h>

// Internal
#include "Extensions/FilterExtension.h"
#include "Extensions/Factories/ExtensionFactory.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// FilterExtensionFactory : ExtensionFactory
	///////////////////////////////////////////////////////////////////////////////
	class FilterExtensionFactory : public ExtensionFactory {
	public:
		FilterExtensionFactory();
		virtual ~FilterExtensionFactory();
		
		virtual FilterExtension* New( const boost::property_tree::ptree::value_type& i_ConfigNode
									 , std::mutex& i_LogMutex
									 , streamlog::severity_log& i_Log ) = 0;
		
	};
	
}

#endif /* defined(__eloquent__FilterExtensionFactory__) */
