#ifndef _Extension_h
#define _Extension_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/property_tree/ptree.hpp>

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// Extension
	///////////////////////////////////////////////////////////////////////////////
	class Extension {
	public:
		Extension() = delete;
		
		explicit Extension( const boost::property_tree::ptree::value_type& i_Config );
		virtual ~Extension() = default;
		
		inline boost::property_tree::ptree::value_type& Config() {
			return m_Config; 
		}
		
	protected:
		// Configuration
		boost::property_tree::ptree::value_type m_Config;

	};

} // namespace Eloquent

#endif // _Extension_h
