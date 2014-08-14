//
//  QueueItem.cpp
//  eloquent
//
//  Created by Nathan Wehr on 8/12/14.
//  Copyright (c) 2014 Creltek. All rights reserved.
//

#include "QueueItem.h"

Eloquent::QueueItem::QueueItem( const std::string& i_Data, const std::string& i_Origin )
: m_Data( i_Data )
, m_Origin( i_Origin )
{}

std::map<void*,bool>& Eloquent::QueueItem::Access() {
	return m_Access;
}

std::string& Eloquent::QueueItem::Data() {
	return m_Data;
}

std::string& Eloquent::QueueItem::Origin() {
	return m_Origin;
}
