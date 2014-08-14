#ifndef __eloquent__FilterExtension__
#define __eloquent__FilterExtension__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <mutex>

// Boost
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/filter/regex.hpp>

// Internal
#include "Extension.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// FilterExtension : Extension
	///////////////////////////////////////////////////////////////////////////////
	class FilterExtension : public Extension, public boost::iostreams::regex_filter {
		FilterExtension();
	public:
		typedef char char_type;
        typedef boost::iostreams::multichar_output_filter_tag category;
		
		explicit FilterExtension( const  boost::property_tree::ptree::value_type& i_Config
								 , std::mutex& i_LogMutex
								 , streamlog::severity_log& i_Log );
		
		virtual ~FilterExtension();
		
		template<typename SinkT>
		std::streamsize write(SinkT& snk, const char* s, std::streamsize n);
		
	};
	
}

#endif /* defined(__eloquent__FilterExtension__) */
