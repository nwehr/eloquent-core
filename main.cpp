//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <iostream>

// Internal
#include "Application.h"

int main( int argc, const char* argv[] ) {
	try {
		return Eloquent::Application::Instance().Init( argc, argv ).Run();	
	} catch( std::exception& e ) {
		std::cerr << e.what() << std::endl;
	}
	
}
