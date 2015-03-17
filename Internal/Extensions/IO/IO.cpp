//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <iostream>

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
, m_SetDestination( m_Config.second.get_optional<std::string>( "set_destination" ) )
, m_IfOrigin( std::vector<boost::optional<std::string>>() )
, m_IfDestination( std::vector<boost::optional<std::string>>() )
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
		
		if( (*it).first == "if_destination" ) {
			m_IfDestination.push_back( boost::optional<std::string>( (*it).second.get_value<std::string>() ) );
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

void Eloquent::IO::PushQueueItem( QueueItem i_QueueItem ) {
	// Add origin
	if( m_SetOrigin.is_initialized() ) {
		i_QueueItem.Origin() = m_SetOrigin.get();
	}
	
	// Add destination
	if( m_SetDestination.is_initialized() ) {
		i_QueueItem.Destination() = m_SetDestination.get();
	}
	
	// Apply filters
	bool FilterSucceeded = true;
	
	for( Filter* F : m_Filters ) {
		if( !(FilterSucceeded = (*F) << i_QueueItem.Data()) ){
			syslog(LOG_DEBUG, "filter failed for queue item #Debug #Reader #Core");
			break;
		}
	}
	
	// Add to the main queue
	if( FilterSucceeded ) {
		if( i_QueueItem.Data().length() ) {
			std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
			m_Queue.push( i_QueueItem );
		}
		
		syslog(LOG_DEBUG, "notifying all writers of a new queue item #Debug #Reader #Core");
		
		// Notify all the writers of a new item
		m_QueueCV.notify_one();
	}
}

Eloquent::QueueItem& Eloquent::IO::NextQueueItem() {
	while( true ) {
		// Wait for new data
		{
			syslog(LOG_DEBUG, "waiting for a queue item #Debug #Writer #Core");
			
			std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
			
			while( !m_Queue.size() || m_Queue.front().Accessed()[this] ) {
				m_QueueCV.wait( QueueLock );
			}
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
		
		// Filter out new data by its destination
		{
			bool AcceptDestination = static_cast<bool>( !m_IfDestination.size() );
			
			// if_destination is set, but we don't know what it is...
			if( !AcceptDestination ) {
				std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
				
				for( boost::optional<std::string>& Destination : m_IfDestination ) {
					if( Destination.get() == "all" || Destination.get() == m_Queue.front().Destination() ) {
						AcceptDestination = true;
						break;
					}
				}
				
				if( !AcceptDestination ) {
					syslog( LOG_DEBUG, "not accepting destination %s #IO::NextQueueItem #Debug", m_Queue.front().Origin().c_str() );
				}
			}
			
			if( !AcceptDestination ) {
				PopQueueItem();
				continue;
			}
		}
		
		// At this point we can get our own copy the data to manipulate/filter
		{
			{
				std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
				m_Item = m_Queue.front();
			}
			
			PopQueueItem();
		}
		
		// Apply filters
		{
			bool FilterDidSucceed = true;
			
			for( Filter* F : m_Filters ) {
				if( !(FilterDidSucceed = (*F) << m_Item.Data()) ) {
					break;
				}
				
			}
			
			if( !FilterDidSucceed ) {
				continue;
			}
		}
		
		// No point in returning an empty string...
		if( !m_Item.Data().length() ) {
			continue;
		}
		
		// Return data
		break;
	}
	
	return m_Item;
}
