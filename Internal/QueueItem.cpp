//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

#include "QueueItem.h"

Eloquent::QueueItem::QueueItem( const std::string& i_Data
							   , const std::string& i_Origin
							   , const std::string& i_Destination )
: m_Data( i_Data )
, m_Origin( i_Origin )
, m_Destination( i_Destination )
{}

Eloquent::QueueItem::QueueItem( const QueueItem& rhs )
: m_Data( rhs.Data() )
, m_Origin( rhs.Origin() )
, m_Destination( rhs.Destination() )
{}

Eloquent::QueueItem& Eloquent::QueueItem::operator=( const QueueItem& rhs ) {
	m_Data = rhs.Data();
	
	m_Origin = rhs.Origin();
	m_Destination = rhs.Destination();
	
	return *this;
	
}
