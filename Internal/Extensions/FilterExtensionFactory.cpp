//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Internal
#include "FilterExtensionFactory.h"

///////////////////////////////////////////////////////////////////////////////
// FilterExtensionFactory : ExtensionFactory
///////////////////////////////////////////////////////////////////////////////
Eloquent::FilterExtensionFactory::FilterExtensionFactory( const std::string& i_Name
														 , const std::string& i_Version
														 , const std::string& i_Author
														 , const std::string& i_Type
														 , const std::vector<std::string>& i_Keys)
: Eloquent::ExtensionFactory( i_Name, i_Version, i_Author, i_Type, i_Keys )
{}

Eloquent::FilterExtensionFactory::~FilterExtensionFactory(){}
