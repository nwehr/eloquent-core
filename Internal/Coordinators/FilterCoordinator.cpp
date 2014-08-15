//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C - C++
#include <deque>
#include <string>

// Boost
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/filter/regex.hpp>
#include <boost/regex.hpp>

// Internal
#include "Extensions/Sink.h"

#include "Extensions/FilterExtension.h"
#include "Extensions/Factories/FilterExtensionFactory.h"

#include "FilterCoordinator.h"

///////////////////////////////////////////////////////////////////////////////
// FilterCoordinator
///////////////////////////////////////////////////////////////////////////////
Eloquent::FilterCoordinator::FilterCoordinator( const boost::property_tree::ptree::value_type& i_Config
												, std::mutex& i_LogMutex
												, streamlog::severity_log& i_Log )
: m_Config( i_Config )
, m_LogMutex( i_LogMutex )
, m_Log( i_Log )
, m_FilteredData( "" )
{
//	Eloquent::Application& App = Eloquent::Application::Instance();
//	std::deque<boost::shared_ptr<Eloquent::ExtensionFactory>>& m_ExtensionFactories = App.Extensions();

	BOOST_FOREACH ( boost::property_tree::ptree::value_type& FilterNode, m_Config.second ) {
		//for( std::deque<boost::shared_ptr<Eloquent::ExtensionFactory>>::iterator it = m_ExtensionFactories.begin(); it != m_ExtensionFactories.end(); ++it ) {
		//	Eloquent::FilterExtensionFactory* FilterFactory = dynamic_cast<Eloquent::FilterExtensionFactory*>( &**it );
		//	if( FilterFactory != 0 ) {
				if( FilterNode.first == "filter" && FilterNode.second.get<std::string>( "name" ) == "regex" ) {
					std::cout << this << " - setting up a regex filter... " << FilterNode.second.get<std::string>( "pattern" ) << std::endl;
					// TODO: kind of obvious...
					boost::regex Pattern( FilterNode.second.get<std::string>( "pattern" ) );
					m_Stream.push( boost::iostreams::regex_filter( Pattern, FilterNode.second.get<std::string>( "replace" ), boost::regex_constants::format_all, boost::regex_constants::format_all ) );
							
				}
				
		//	}
			
		// }

	}

//	std::cout << "Complete: " << m_Stream.is_complete() << std::endl;
//	std::cout << "Strict Sync: " << m_Stream.strict_sync() << std::endl;
	
} 

Eloquent::FilterCoordinator::~FilterCoordinator(){}

const std::string& Eloquent::FilterCoordinator::FilterData( const std::string& i_Data ) {
	m_FilteredData = "";

//	std::cout << this << " - FilterCoordinator::FilterData() i_Data - " << i_Data << std::endl;
//	std::cout << this << " - FilterCoordinator::FilterData() m_FilteredData (before stream) - " << m_FilteredData << std::endl;
	// std::cout << "Complete: " << m_Stream.is_complete() << std::endl;

	m_Stream.push( Sink( m_FilteredData ) );
	m_Stream << i_Data;
	m_Stream.pop();

//	std::cout << this << " - FilterCoordinator::FilterData() m_FilteredData (after stream) - " << m_FilteredData << std::endl;

	return m_FilteredData;

}
