#ifndef eloquentd_Filter_h
#define eloquentd_Filter_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/property_tree/ptree.hpp>

// Internal
#include "Extensions/Extension.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// Filter
	///////////////////////////////////////////////////////////////////////////////
	class Filter : public Extension {
	public:
		explicit Filter( const boost::property_tree::ptree::value_type& i_Config
						, std::mutex& i_LogMutex
						, streamlog::severity_log& i_Log );
		
		virtual ~Filter();
		
		virtual std::string& operator<<( std::string& ) = 0;
		
	};

}

#endif
