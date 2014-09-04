//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Internal
#include "Extensions/IO/IO.h"
#include "Extensions/Filters/Filter.h"
#include "Extensions/Filters/FilterFactory.h"
#include "Extensions/ExtensionManager.h"

///////////////////////////////////////////////////////////////////////////////
// IOExtension : Extension
///////////////////////////////////////////////////////////////////////////////
Eloquent::IO::IO( const  boost::property_tree::ptree::value_type& i_Config
				 , std::mutex& i_QueueMutex
				 , std::condition_variable& i_QueueCV
				 , std::queue<QueueItem>& i_Queue
				 , unsigned int& i_NumWriters )
: Eloquent::Extension( i_Config )
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
			FilterFactory* MyFilterFactory = reinterpret_cast<FilterFactory*>( ExtensionManager::Instance().LoadExtension( (*it).second.get<std::string>( "load" ) ) );
			
			m_Filters.push_back( MyFilterFactory->New( boost::property_tree::ptree::value_type( *it ) ) );
			
		}
		
	}

}

boost::optional<std::string>& Eloquent::IO::SetOrigin() {
	return m_SetOrigin;
}

std::vector<boost::optional<std::string>>& Eloquent::IO::IfOrigin() {
	return m_IfOrigin;
}

void Eloquent::IO::PopQueueItem() {
	std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
	
	// Mark this item as read by us
	m_Queue.front().Accessed()[this] = true;
	
	// Make sure all the writers have read this item before removing it
	if( m_Queue.front().Accessed().size() == m_NumWriters ) {
		m_Queue.pop();
	}
	
}

void Eloquent::IO::PushQueueItem( QueueItem& i_QueueItem ) {
	// Apply filters
	for( Filter* F : m_Filters ) {
		(*F) << i_QueueItem.Data();
	}
	
	// Add to the main queue
	if( i_QueueItem.Data().length() ) {
		std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
		m_Queue.push( i_QueueItem );
	}
	
	// Notify all the writers of a new item
	m_QueueCV.notify_one();
	
}

void Eloquent::IO::PushQueueItem( const QueueItem& i_Item ) {
	QueueItem Item = i_Item;
	PushQueueItem( Item );
}

Eloquent::QueueItem& Eloquent::IO::NextQueueItem() {
	while( true ) {
		// Wait for new data
		{
			std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
			
			while( !m_Queue.size() || m_Queue.front().Accessed()[this] ) {
				m_QueueCV.wait( QueueLock );
			}
			
			syslog( LOG_DEBUG, "fetching 1 of %lu item(s) #IO::NextQueueItem #Debug", m_Queue.size() );
			
		}
		
		// Filter out new data by its origin
		{
			bool AcceptOrigin = static_cast<bool>( !m_IfOrigin.size() );
			
			// if_origin is set, but we don't know what it is...
			if( !AcceptOrigin ) {
				std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
				
				for( boost::optional<std::string>& Origin : m_IfOrigin ) {
					if( Origin.get() == "all" || Origin.get() == m_Queue.front().Origin() ) {
						AcceptOrigin = true;
						break;
					}
					
				}
				
				if( !AcceptOrigin ) {
					syslog( LOG_DEBUG, "not accepting origin %s #IO::NextQueueItem #Debug", m_Queue.front().Origin().c_str() );
				}
				
			}
			
			if( !AcceptOrigin ) {
				PopQueueItem();
				continue;
			}
			
		}
		
		// Apply filters
		{
			std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
			
			for( Filter* F : m_Filters ) {
				(*F) << m_Queue.front().Data();
			}
			
		}
		
		// Any data left to return?
		{
			bool EmptyData = false;
			
			{
				std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
				
				if( !m_Queue.front().Data().length() ) {
					syslog( LOG_DEBUG, "data is %lu bytes long #IO::NextQueueItem #Debug", m_Queue.size() );
					EmptyData = true;
				}
				
			}
			
			if( EmptyData ) {
				PopQueueItem();
				continue;
			}
			
		}
		
		// Return data
		break;

	}
	
	return m_Queue.front();
	
}
