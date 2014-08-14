//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Internal
#include "Sink.h"

///////////////////////////////////////////////////////////////////////////////
// Sink
///////////////////////////////////////////////////////////////////////////////
Eloquent::Sink::Sink( std::string& i_Data )
: m_Data( i_Data )
{}

std::streamsize Eloquent::Sink::write( const char* s, std::streamsize n ) {
	m_Data.insert( m_Data.end(), s, s + n );
	return n;
}
