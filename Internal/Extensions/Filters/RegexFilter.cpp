// C - C++
#include <string.h>
#include <iosfwd>

// Internal
#include "RegexFilter.h"

struct my_formatter {
	std::string operator()(const boost::match_results<const char*>& match)
	{
		// Examine match and return the appropriate replacement text
	}
};

///////////////////////////////////////////////////////////////////////////////
// RegexFilter : FilterExtension
///////////////////////////////////////////////////////////////////////////////
Eloquent::RegexFilter::RegexFilter( const  boost::property_tree::ptree::value_type& i_Config
										   , std::mutex& i_LogMutex
										   , streamlog::severity_log& i_Log )
: Eloquent::FilterExtension( i_Config, i_LogMutex, i_Log )
, boost::iostreams::regex_filter(<#args#>)
{}

Eloquent::RegexFilter::~RegexFilter() {}

std::streamsize Eloquent::RegexFilter::write( const char* s, std::streamsize n ) {
//	boost::match_results<std::string::const_iterator> Results;
//
//	if( boost::regex_match( std::string( s ), Results, m_Pattern ) ) {
//		std::cout << "FOUND REGEX MATCH!" << std::endl;
//		std::streamsize z = Results[1].str().size();
//		boost::iostreams::put( dest, Results[1].str() );
//
//		return z;
//
//	}
//	
//	boost::iostreams::put( dest, "" );

	return 0;
}