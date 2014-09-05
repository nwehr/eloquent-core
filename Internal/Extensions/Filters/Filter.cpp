// Internal
#include "Filter.h"

///////////////////////////////////////////////////////////////////////////////
// Filter
///////////////////////////////////////////////////////////////////////////////
Eloquent::Filter::Filter( const boost::property_tree::ptree::value_type& i_Config )
: Extension( i_Config )
, m_ApplyNextFilterOnSuccess( m_Config.second.get_optional<bool>( "next_filter_on_success" ) )
, m_ApplyNextFilterOnFailure( m_Config.second.get_optional<bool>( "next_filter_on_failure" ) )
{}
