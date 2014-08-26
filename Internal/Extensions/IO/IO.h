#ifndef __eloquent__IOExtension__
#define __eloquent__IOExtension__

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

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
		IO();
	public:
		explicit IO( const  boost::property_tree::ptree::value_type& i_Config
					, std::mutex& i_LogMutex
					, streamlog::severity_log& i_Log
					, std::mutex& i_QueueMutex
					, std::condition_variable& i_QueueCV
					, std::queue<QueueItem>& i_Queue
					, int& i_NumWriters );
		
		virtual ~IO();
		
		virtual void operator()() = 0;
		
		boost::optional<std::string>& SetOrigin();
		std::vector<boost::optional<std::string>>& IfOrigin();
		
		void PopQueueItem();
		void PushQueueItem( const QueueItem& );
		QueueItem& NextQueueItem();
		
		std::mutex& QueueMutex();
		std::condition_variable& QueueCV();
		std::queue<QueueItem>& Queue();
		
	protected:
		boost::optional<std::string> m_SetOrigin;
		std::vector<boost::optional<std::string>> m_IfOrigin;
		
		// Shared Memory
		std::mutex&					m_QueueMutex;
		std::condition_variable&	m_QueueCV;
		std::queue<QueueItem>&		m_Queue;
		int&						m_NumWriters;
		
		// Filters
		std::deque<Filter*>	m_Filters;
		
	};
	
}

#endif /* defined(__eloquent__IOExtension__) */
