//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Internal
#include "ExtensionFactory.h"

///////////////////////////////////////////////////////////////////////////////
// ExtensionFactory
///////////////////////////////////////////////////////////////////////////////
Eloquent::ExtensionFactory::ExtensionFactory( const std::string& i_Name
						  , const std::string& i_Version
						  , const std::string& i_Author
						  , const std::string& i_Type
						  , const std::vector<std::string>& i_Keys )
: m_Name( i_Name )
, m_Version( i_Version )
, m_Author( i_Author )
, m_Type( i_Type )
, m_Keys( i_Keys )
{}

Eloquent::ExtensionFactory::~ExtensionFactory(){}

const std::string& Eloquent::ExtensionFactory::Name() const {
	return m_Name;
}

const std::string& Eloquent::ExtensionFactory::Version() const {
	return m_Version;
}

const std::string& Eloquent::ExtensionFactory::Author() const {
	return m_Author;
}

const std::string& Eloquent::ExtensionFactory::Type() const {
	return m_Type;
}

const std::vector<std::string>& Eloquent::ExtensionFactory::Keys() const {
	return m_Keys;
}
