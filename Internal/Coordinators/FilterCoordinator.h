#ifndef _FilterCoordinator_h
#define _FilterCoordinator_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <iostream>
#include <string>
#include <mutex>

// Boost
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>

#include <boost/regex.hpp>

namespace Eloquent {
	///////////////////////////////////////////////////////////////////////////////
	// FilterCoordinator
	///////////////////////////////////////////////////////////////////////////////
	class FilterCoordinator {
	public:
		FilterCoordinator( const boost::property_tree::ptree::value_type& i_Config
							, std::mutex& i_LogMutex
							, streamlog::severity_log& i_Log );

		~FilterCoordinator();

		const std::string& FilterData( const std::string& m_Data );

	private:
		// Configuration 
		boost::property_tree::ptree::value_type m_Config;

		// Logging
		std::mutex&			 m_LogMutex;
		streamlog::severity_log& m_Log;


		std::string m_FilteredData;
		boost::iostreams::filtering_ostream m_Stream;
		
	};

}

#endif // _FilterCoordinator_h
