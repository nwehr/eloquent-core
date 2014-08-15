#ifndef _Extension_h
#define _Extension_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <mutex>

// Boost
#include <boost/property_tree/ptree.hpp>

// External
#include <streamlog/streamlog.h>

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// Extension
	///////////////////////////////////////////////////////////////////////////////
	class Extension {
		Extension(); 
	public:
		explicit Extension( const  boost::property_tree::ptree::value_type& i_Config
						   	, std::mutex& i_LogMutex
						   	, streamlog::severity_log& i_Log );

		virtual ~Extension();
		
		boost::property_tree::ptree::value_type& Config();
		
		std::mutex& LogMutex();
		streamlog::severity_log& Log();

	protected:
		// Configuration
		boost::property_tree::ptree::value_type m_Config;

		// Logging
		std::mutex&			 m_LogMutex;
		streamlog::severity_log& m_Log;
	};

} // namespace Eloquent

#endif // _Extension_h
