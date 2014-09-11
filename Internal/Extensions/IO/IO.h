#ifndef __eloquent__IOExtension__
#define __eloquent__IOExtension__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C
#include <syslog.h>

// C++
#include <queue>
#include <mutex>
#include <condition_variable>

// Internal
#include "Extensions/Extension.h"
#include "Extensions/Filters/Filter.h"

#include "QueueItem.h"

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// IO : Extension
	///////////////////////////////////////////////////////////////////////////////
	class IO : public Extension {
	public:
		IO() = delete;
		IO& operator=( const IO& ) = delete;
		IO( const IO& ) = delete; 
		
		explicit IO( const  boost::property_tree::ptree::value_type& i_Config
					, std::mutex& i_QueueMutex
					, std::condition_variable& i_QueueCV
					, std::queue<QueueItem>& i_Queue
					, unsigned int& i_NumWriters );
		
		virtual ~IO() = default;
		
		virtual void operator()() = 0;
		
		boost::optional<std::string>& SetOrigin();
		boost::optional<std::string>& SetDestination();
		
		std::vector<boost::optional<std::string>>& IfOrigin();
		std::vector<boost::optional<std::string>>& IfDestination();
		
		void PopQueueItem();
		
		void PushQueueItem( QueueItem& );
		void PushQueueItem( const QueueItem& );
		
		QueueItem& NextQueueItem();
		
		inline std::mutex& QueueMutex() { return m_QueueMutex; }
		inline std::condition_variable& QueueCV() { return m_QueueCV; }
		inline std::queue<QueueItem>& Queue() { return m_Queue; }
		
	protected:
		boost::optional<std::string> m_SetOrigin;
		boost::optional<std::string> m_SetDestination;
		
		std::vector<boost::optional<std::string>> m_IfOrigin;
		std::vector<boost::optional<std::string>> m_IfDestination;
		
		// Shared Memory
		std::mutex&					m_QueueMutex;
		std::condition_variable&	m_QueueCV;
		std::queue<QueueItem>&		m_Queue;
		unsigned int&				m_NumWriters;
		
		// Filters
		std::deque<Filter*>	m_Filters;
		
		// Data
		QueueItem m_Item; 
		
	};
	
}

#endif /* defined(__eloquent__IOExtension__) */
