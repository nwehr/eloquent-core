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
	public:
		QueueItem() = delete;
		
		QueueItem& operator=( const QueueItem& ) = default;
		QueueItem( const QueueItem& ) = default;
		
		explicit QueueItem( const std::string& i_Data, const std::string& i_Origin );
		
		virtual ~QueueItem() = default;
		
		inline std::map<void*,bool>& Accessed()	{ return m_Accessed; }
		inline const std::map<void*,bool>& Accessed() const { return m_Accessed; }
		
		inline std::string& Data() { return m_Data; }
		inline const std::string& Data() const { return m_Data; }
		
		inline std::string& Origin() { return m_Origin; }
		inline const std::string& Origin() const { return m_Origin; }
		
	private:
		std::map<void*,bool> m_Accessed;
		std::string m_Data;
		std::string m_Origin;
		
	};
}


#endif /* defined(__eloquent__QueueItem__) */
