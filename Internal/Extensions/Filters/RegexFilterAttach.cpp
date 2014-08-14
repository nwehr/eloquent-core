#include "RegexFilterAttach.h"

namespace Eloquent {
	static const std::string EXTENSION_NAME		= "regex";
	static const std::string EXTENSION_VERSION	= "1.0";
	static const std::string EXTENSION_AUTHOR	= "Donald Cluck <donaldc@evrichart.com>";
	static const std::string EXTENSION_TYPE		= "filter";
	
	static std::vector<std::string> EXTENSION_KEYS;

}

///////////////////////////////////////////////////////////////////////////////
// Library Initializer
///////////////////////////////////////////////////////////////////////////////
extern "C" boost::shared_ptr<Eloquent::ExtensionFactory> Attach( void ) {
	Eloquent::EXTENSION_KEYS.push_back( "location" );
	Eloquent::EXTENSION_KEYS.push_back( "format" );
	
	return boost::shared_ptr<Eloquent::ExtensionFactory>( reinterpret_cast<Eloquent::ExtensionFactory*>( new Eloquent::RegexFilterFactory( Eloquent::EXTENSION_NAME, Eloquent::EXTENSION_VERSION, Eloquent::EXTENSION_AUTHOR, Eloquent::EXTENSION_TYPE, Eloquent::EXTENSION_KEYS ) ) );
}
