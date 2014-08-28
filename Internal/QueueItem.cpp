//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

#include "QueueItem.h"

Eloquent::QueueItem::QueueItem( const std::string& i_Data, const std::string& i_Origin )
: m_Data( i_Data )
, m_Origin( i_Origin )
{}

std::map<void*,bool>& Eloquent::QueueItem::Accessed() {
	return m_Accessed; 
}

const std::map<void*,bool>& Eloquent::QueueItem::Accessed() const {
	return m_Accessed; 
}

std::string& Eloquent::QueueItem::Data() {
	return m_Data;
}

const std::string& Eloquent::QueueItem::Data() const {
	return m_Data;
}

std::string& Eloquent::QueueItem::Origin() {
	return m_Origin;
}

const std::string& Eloquent::QueueItem::Origin() const {
	return m_Origin;
}
