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
		QueueItem() = default;
		
		QueueItem& operator=( const QueueItem& );
		QueueItem( const QueueItem& ); 
		
		explicit QueueItem( const std::string& i_Data
						   , const std::string& i_Origin = ""
						   , const std::string& i_Destination = "" );
		
		virtual ~QueueItem() = default;
		
		inline std::map<void*,bool>& Accessed()	{ return m_Accessed; }
		inline const std::map<void*,bool>& Accessed() const { return m_Accessed; }
		
		inline std::string& Data() { return m_Data; }
		inline const std::string& Data() const { return m_Data; }
		
		inline std::string& Origin() { return m_Origin; }
		inline const std::string& Origin() const { return m_Origin; }
		
		inline std::string& Destination() { return m_Destination; }
		inline const std::string& Destination() const { return m_Destination; }
		
	private:
		std::map<void*,bool> m_Accessed;
		std::string m_Data;
		
		std::string m_Origin;
		std::string m_Destination;
		
	};
}


#endif /* defined(__eloquent__QueueItem__) */
