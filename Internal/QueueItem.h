#ifndef __eloquent__QueueItem__
#define __eloquent__QueueItem__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <string>
#include <map>

namespace Eloquent {
	class QueueItem {
		QueueItem();
		
	public:
		QueueItem( const std::string& i_Data, const std::string& i_Origin );
		
		std::map<void*,bool>& Access() __attribute__ ((deprecated));
		std::map<void*,bool>& Accessed();
		
		std::string& Data();
		std::string& Origin();
		
	private:
		std::map<void*,bool> m_Accessed;
		std::string m_Data;
		std::string m_Origin;
		
	};
}


#endif /* defined(__eloquent__QueueItem__) */
