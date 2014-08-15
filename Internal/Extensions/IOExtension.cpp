//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Boost
#include <boost/smart_ptr.hpp>

// Internal
#include "IOExtension.h"

///////////////////////////////////////////////////////////////////////////////
// IOExtension : Extension
///////////////////////////////////////////////////////////////////////////////
Eloquent::IOExtension::IOExtension( const  boost::property_tree::ptree::value_type& i_Config
								   , std::mutex& i_LogMutex
								   , streamlog::severity_log& i_Log
								   , std::mutex& i_QueueMutex
								   , std::condition_variable& i_QueueCV
								   , std::queue<QueueItem>& i_Queue
								   , int& i_NumWriters)
: Eloquent::Extension( i_Config, i_LogMutex, i_Log )
, m_FilterCoordinator( new Eloquent::FilterCoordinator( i_Config, i_LogMutex, i_Log ) )
, m_SetOrigin( m_Config.second.get_optional<std::string>( "set_origin" ) )
, m_IfOrigin( std::vector<boost::optional<std::string>>()  )
, m_QueueMutex( i_QueueMutex )
, m_QueueCV( i_QueueCV )
, m_Queue( i_Queue )
, m_NumWriters( i_NumWriters )
{
	for( boost::property_tree::ptree::iterator it = m_Config.second.begin(); it != m_Config.second.end(); ++it ) {
		if( (*it).first == "if_origin" ) {
			m_IfOrigin.push_back( boost::optional<std::string>( (*it).second.get_value<std::string>() ) );
		}
		
	}

}

Eloquent::IOExtension::~IOExtension(){
	delete m_FilterCoordinator;
}

Eloquent::FilterCoordinator*  Eloquent::IOExtension::FilterCoord() {
	return m_FilterCoordinator;
}

boost::optional<std::string>& Eloquent::IOExtension::SetOrigin() {
	return m_SetOrigin;
}

std::vector<boost::optional<std::string>>& Eloquent::IOExtension::IfOrigin() {
	return m_IfOrigin;
}

void Eloquent::IOExtension::PopQueueItem() {
	std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
	
	m_Queue.front().Accessed()[this] = true;
	
	if( m_Queue.front().Accessed().size() == m_NumWriters ) {
		m_Queue.pop();
	}
	
}

Eloquent::QueueItem& Eloquent::IOExtension::NextQueueItem() {
	while( true ) {
		bool DoReturnItem = static_cast<bool>( !m_IfOrigin.size() );
		
		{
			std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
			
			while( !m_Queue.size() || m_Queue.front().Accessed()[this] ) {
				m_QueueCV.wait( QueueLock );
			}
			
			if( m_IfOrigin.size() ) {
				for( int i = 0; i < m_IfOrigin.size(); ++i ) {
					if( m_IfOrigin[i].get() == "all" || m_IfOrigin[i].get() == m_Queue.front().Origin() ) {
						DoReturnItem = true; break;
					}
					
				}
				
			}
			
		}
		
		if( !DoReturnItem ) {
			PopQueueItem();
		} else {
			break;
		}
		
	}
	
	std::unique_lock<std::mutex> QueueLock( m_QueueMutex );
	return m_Queue.front();
	
}

std::mutex& Eloquent::IOExtension::QueueMutex() {
	return m_QueueMutex;
}

std::condition_variable& Eloquent::IOExtension::QueueCV() {
	return m_QueueCV;
}

std::queue<Eloquent::QueueItem>& Eloquent::IOExtension::Queue() {
	return m_Queue;
}
