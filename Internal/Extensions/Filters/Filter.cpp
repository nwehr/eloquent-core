// Internal
#include "Filter.h"

///////////////////////////////////////////////////////////////////////////////
// Filter
///////////////////////////////////////////////////////////////////////////////
Eloquent::Filter::Filter( const boost::property_tree::ptree::value_type& i_Config )
: Extension( i_Config )
, m_ContinueOnFail( m_Config.second.get_optional<bool>( "continue_on_fail" ) )
{}

bool Eloquent::Filter::Continue( bool i_Succeeded ) {
	if( !i_Succeeded ) {
		if( m_ContinueOnFail.is_initialized() && m_ContinueOnFail.get() ) {
			return true;
		}
		
		return false;
		
	}
	
	return true;
	
}
