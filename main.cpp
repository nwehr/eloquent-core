//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// Internal
#include "Application.h"

int main( int argc, const char* argv[] ){
	return Eloquent::Application::Instance().Init( argc, argv ).Run();
}
