#include "RegexFilter.h"
#include "RegexFilterFactory.h"

///////////////////////////////////////////////////////////////////////////////
// RegexFilterFactory : FilterExtensionFactory
///////////////////////////////////////////////////////////////////////////////
Eloquent::RegexFilterFactory::RegexFilterFactory( const std::string& i_Name
											   , const std::string& i_Version
											   , const std::string& i_Author
											   , const std::string& i_Type
											   , const std::vector<std::string>& i_Keys )
: FilterExtensionFactory( i_Name, i_Version, i_Author, i_Type, i_Keys )
{}

Eloquent::RegexFilterFactory::~RegexFilterFactory(){}

Eloquent::FilterExtension* Eloquent::RegexFilterFactory::New( const boost::property_tree::ptree::value_type& i_Config
														, boost::mutex& i_LogMutex
														, streamlog::severity_log& i_Log )
{
	return new RegexFilter( i_Config, i_LogMutex, i_Log );
}