#ifndef __eloquent__Sink__
#define __eloquent__Sink__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/iostreams/concepts.hpp>

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// Sink
	///////////////////////////////////////////////////////////////////////////////
	class Sink : public boost::iostreams::sink {
	public:
		Sink( std::string& i_Data );
		
		std::streamsize write( const char* s, std::streamsize n );
		
	private:
		std::string& m_Data;
		
	};
}

#endif /* defined(__eloquent__Sink__) */
