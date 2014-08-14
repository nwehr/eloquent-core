//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C
#include <iosfwd>

// C++
#include <string.h>

// Boost
#include <boost/regex.hpp>

// Internal
#include "FilterExtension.h"

struct my_formatter {
	std::string operator()(const boost::match_results<const char*>& match)
	{
		// Examine match and return the appropriate replacement text
		return ""; 
	}
};

///////////////////////////////////////////////////////////////////////////////
// FilterExtension : Extension
///////////////////////////////////////////////////////////////////////////////
Eloquent::FilterExtension::FilterExtension( const  boost::property_tree::ptree::value_type& i_Config
										   , std::mutex& i_LogMutex
										   , streamlog::severity_log& i_Log )
: Extension( i_Config, i_LogMutex, i_Log )
, boost::iostreams::regex_filter( boost::regex( "(.*)" ), my_formatter() )
{}

Eloquent::FilterExtension::~FilterExtension() {}

template<typename SinkT>
std::streamsize write(SinkT& snk, const char* s, std::streamsize n) {
	return 0;
}
