#ifndef _Logging_h
#define _Logging_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <string>

namespace Eloquent {
	namespace LogSeverity {
		static const int SEV_TRACE		= 0x0;
		static const int SEV_DEBUG		= 0x1;
		static const int SEV_INFO		= 0x2;
		static const int SEV_WARNING	= 0x3;
		static const int SEV_ERROR		= 0x4;
		static const int SEV_FATAL		= 0x5;
	}

}

#endif // _Logging_h