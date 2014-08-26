// Internal
#include "Extension.h"

///////////////////////////////////////////////////////////////////////////////
// Extension
///////////////////////////////////////////////////////////////////////////////
Eloquent::Extension::Extension(	 const boost::property_tree::ptree::value_type& i_Config
				   				, std::mutex& i_LogMutex
				   				, streamlog::severity_log& i_Log )
: m_Config( i_Config )
, m_LogMutex( i_LogMutex )
, m_Log( i_Log )
{}

Eloquent::Extension::~Extension(){}

boost::property_tree::ptree::value_type& Eloquent::Extension::Config() {
	return m_Config;
}

std::mutex& Eloquent::Extension::LogMutex() {
	return m_LogMutex;
}

streamlog::severity_log& Eloquent::Extension::Log() {
	return m_Log; 
}
