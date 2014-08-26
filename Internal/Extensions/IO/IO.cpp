//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/smart_ptr.hpp>

// Internal
#include "Extensions/IO/IO.h"
#include "Extensions/Filters/Filter.h"
#include "Extensions/Filters/FilterFactory.h"
#include "Extensions/ExtensionManager.h"

///////////////////////////////////////////////////////////////////////////////
// IOExtension : Extension
///////////////////////////////////////////////////////////////////////////////
Eloquent::IO::IO( const  boost::property_tree::ptree::value_type& i_Config
				 , std::mutex& i_LogMutex
				 , streamlog::severity_log& i_Log
				 , std::mutex& i_QueueMutex
				 , std::condition_variable& i_QueueCV
				 , std::queue<QueueItem>& i_Queue
				 , int& i_NumWriters )
: Eloquent::Extension( i_Config, i_LogMutex, i_Log )
, m_SetOrigin( m_Config.second.get_optional<std::string>( "set_origin" ) )
, m_IfOrigin( std::vector<boost::optional<std::string>>()  )
, m_QueueMutex( i_QueueMutex )
, m_QueueCV( i_QueueCV )
, m_Queue( i_Queue )
, m_NumWriters( i_NumWriters )
, m_Filters( std::deque<Filter*>() )
{
	for( boost::property_tree::ptree::iterator it = m_Config.second.begin(); it != m_Config.second.end(); ++it ) {
		if( (*it).first == "if_origin" ) {
			m_IfOrigin.push_back( boost::optional<std::string>( (*it).second.get_value<std::string>() ) );
		}
		
		if( (*it).first == "filter" ) {
			FilterFactory* MyFilterFactory = reinterpret_cast<FilterFactory*>( ExtensionManager::Instance( m_LogMutex, m_Log ).LoadExtension( (*it).second.get<std::string>( "load" ) ) );
			
			m_Filters.push_back( MyFilterFactory->New( boost::property_tree::ptree::value_type( *it ), m_LogMutex, m_Log ) );
			
		}
		
	}

}

Eloquent::IO::~IO(){}

boost::optional<std::string>& Eloquent::IO::SetOrigin() {
	return m_SetOrigin;
}

std::vector<boost::optional<std::string>>& Eloquent::IO::IfOrigin() {
	return m_IfOrigin;
}

void Eloquent::IO::PopQueueItem() {
	std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
	
	m_Queue.front().Accessed()[this] = true;
	
	if( m_Queue.front().Accessed().size() == m_NumWriters ) {
		m_Queue.pop();
	}
	
}

void Eloquent::IO::PushQueueItem( const QueueItem& i_QueueItem ) {
	QueueItem Item = i_QueueItem;
	
	if( m_Filters.size() ) {
		for( Filter* F : m_Filters ) {
			F->operator<<( Item.Data() );
		}
		
	}
	
	{
		std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
		m_Queue.push( Item );
	}
	
	m_QueueCV.notify_one();
	
}

Eloquent::QueueItem& Eloquent::IO::NextQueueItem() {
	while( true ) {
		bool DoReturnItem = static_cast<bool>( !m_IfOrigin.size() );
		
		{
			std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
			
			while( !m_Queue.size() || m_Queue.front().Accessed()[this] ) {
				m_QueueCV.wait( QueueLock );
			}
			
			if( m_IfOrigin.size() ) {
				for( boost::optional<std::string>& Origin : m_IfOrigin ) {
					if( Origin.get() == "all" || Origin.get() == m_Queue.front().Origin() ) {
						DoReturnItem = true; break;
					}
					
				}
				
			}
			
		}
		
		if( !DoReturnItem ) {
			PopQueueItem();
		} else {
			if( m_Filters.size() ) {
				for( Filter* F : m_Filters ) {
					std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
					F->operator<<( m_Queue.front().Data() );
					
				}
				
			}
			
			std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
			
			if( m_Queue.front().Data().length() )
				break;
			
		}
		
	}
	
	return m_Queue.front();
	
}

std::mutex& Eloquent::IO::QueueMutex() {
	return m_QueueMutex;
}

std::condition_variable& Eloquent::IO::QueueCV() {
	return m_QueueCV;
}

std::queue<Eloquent::QueueItem>& Eloquent::IO::Queue() {
	return m_Queue;
}
