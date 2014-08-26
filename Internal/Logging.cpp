//
//  Logging.cpp
//  eloquentd
//
//  Created by Nathan Wehr on 8/25/14.
//  Copyright (c) 2014 EvriChart, Inc. All rights reserved.
//

// C++
#include <sstream>

// Internal
#include "Logging.h"

std::string Eloquent::TimeAndSpace() {
	std::stringstream MyStream;
	
	// Add the timestamp
	time_t CurrentTime = time( 0 );
	struct tm* LocalTime = localtime( &CurrentTime );
	
	char FormattedTime[32] = { "" };
	
	strftime( FormattedTime, 32, "%F %T %z", LocalTime );
	
	MyStream << "[" << FormattedTime << "] ";
	
	// Add the host name
	if( !Eloquent::Hostname.length() ) {
		char hn[256] = { "" };
		gethostname( hn, 256 );
		
		Eloquent::Hostname = hn;
		
	}
	
	MyStream << Eloquent::Hostname << " ";
	
	// Add the PID
	if( !Eloquent::ProcessID ) {
		Eloquent::ProcessID = getpid();
	}
	
	MyStream << Eloquent::ProcessID << ": ";
	
	return MyStream.str();
	
}
