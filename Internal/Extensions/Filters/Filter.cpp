// Internal
#include "Filter.h"

///////////////////////////////////////////////////////////////////////////////
// Filter
///////////////////////////////////////////////////////////////////////////////
Eloquent::Filter::Filter( const boost::property_tree::ptree::value_type& i_Config
				 , std::mutex& i_LogMutex
				 , streamlog::severity_log& i_Log )
: Extension( i_Config, i_LogMutex, i_Log )
{}

Eloquent::Filter::~Filter() {}
