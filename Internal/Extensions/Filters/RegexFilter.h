#ifndef _RegexFilter_h
#define _RegexFilter_h

// Boost
#include <boost/iostreams/filter/regex.hpp>
#include <boost/regex.hpp>

// Internal
#include "Extensions/FilterExtension.h"

namespace Eloquent {
	class RegexFilter : public FilterExtension, public boost::iostreams::regex_filter {
	public:
		explicit RegexFilter( const  boost::property_tree::ptree::value_type& i_Config
					 		, std::mutex& i_LogMutex
					 		, streamlog::severity_log& i_Log );

		virtual ~RegexFilter();

	};
}

#endif // _RegexFilter_h