#ifndef __eloquent__FilterExtensionFactory__
#define __eloquent__FilterExtensionFactory__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

// External
#include <streamlog/streamlog.h>

// Internal
#include "Extensions/FilterExtension.h"
#include "Extensions/ExtensionFactory.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// FilterExtensionFactory : ExtensionFactory
	///////////////////////////////////////////////////////////////////////////////
	class FilterExtensionFactory : public ExtensionFactory {
	public:
		explicit FilterExtensionFactory( const std::string& i_Name
						  				, const std::string& i_Version
						  				, const std::string& i_Author
						  				, const std::string& i_Type
						  				, const std::vector<std::string>& i_Keys);
		
		virtual ~FilterExtensionFactory();
		
		virtual FilterExtension* New( const boost::property_tree::ptree::value_type& i_ConfigNode
									 , std::mutex& i_LogMutex
									 , streamlog::severity_log& i_Log ) = 0;
		
	};
	
}

#endif /* defined(__eloquent__FilterExtensionFactory__) */
