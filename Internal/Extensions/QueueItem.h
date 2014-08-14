//
//  QueueItem.h
//  eloquent
//
//  Created by Nathan Wehr on 8/12/14.
//  Copyright (c) 2014 Creltek. All rights reserved.
//

#ifndef __eloquent__QueueItem__
#define __eloquent__QueueItem__

// C++
#include <string>
#include <map>

namespace Eloquent {
	class QueueItem {
		QueueItem();
		
	public:
		QueueItem( const std::string& i_Data, const std::string& i_Origin );
		
		std::map<void*,bool>& Access();
		std::string& Data();
		std::string& Origin();
	private:
		std::map<void*,bool> m_Access;
		std::string m_Data;
		std::string m_Origin;
		
	};
}


#endif /* defined(__eloquent__QueueItem__) */
