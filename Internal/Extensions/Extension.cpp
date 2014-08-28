// Internal
#include "Extension.h"

///////////////////////////////////////////////////////////////////////////////
// Extension
///////////////////////////////////////////////////////////////////////////////
Eloquent::Extension::Extension(	const boost::property_tree::ptree::value_type& i_Config )
: m_Config( i_Config )
{}

Eloquent::Extension::~Extension(){}

boost::property_tree::ptree::value_type& Eloquent::Extension::Config() {
	return m_Config;
}
