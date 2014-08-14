#ifndef __eloquent__RegexFilterFactory__
#define __eloquent__RegexFilterFactory__

// C++
#include <vector>
#include <string>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

// Internal
#include "Extensions/FilterExtensionFactory.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// RegexFilterFactory : FilterExtensionFactory
	///////////////////////////////////////////////////////////////////////////////
	class RegexFilterFactory : public FilterExtensionFactory {
		RegexFilterFactory();
	public:
		explicit RegexFilterFactory( const std::string& i_Name
								   , const std::string& i_Version
								   , const std::string& i_Author
								   , const std::string& i_Type
								   , const std::vector<std::string>& i_Keys );
		
		virtual ~RegexFilterFactory();
		
		virtual FilterExtension* New( const boost::property_tree::ptree::value_type& i_Config
								 , boost::mutex& i_LogMutex
								 , streamlog::severity_log& i_Log );
		
	};
}

#endif /* defined(__eloquent__RegexFilterFactory__) */