// C
#include <dlfcn.h>
#include <syslog.h>

// C++
#include <sstream>
#include <functional>

// Internal
#include "ExtensionManager.h"

Eloquent::ExtensionManager* Eloquent::ExtensionManager::m_Instance;

Eloquent::ExtensionManager::~ExtensionManager() {
	for( std::tuple<void*, std::string, ExtensionFactory*>& Extension : m_Extensions ) {
		delete std::get<2>( Extension );
		dlclose( std::get<0>( Extension ) );
	}
	
}
		
Eloquent::ExtensionManager& Eloquent::ExtensionManager::Instance() {
	if( !m_Instance )
		m_Instance = new Eloquent::ExtensionManager();
	
	return *m_Instance;

}

Eloquent::ExtensionFactory* Eloquent::ExtensionManager::LoadExtension( boost::filesystem::path i_ExtensionPath ) {
	try {
		// Have we already loaded an extension for the given path? If so, return the factory object for it
		for( std::tuple<void*, std::string, ExtensionFactory*>& Extension : m_Extensions ) {
			if( i_ExtensionPath.string() == std::get<1>( Extension ) ) {
				return std::get<2>( Extension );
			}
			
		}
		
		// Does this path even exist?
		if( !boost::filesystem::exists( i_ExtensionPath ) ) {
			syslog( LOG_ERR, "unable to find extension at %s #ExtensionManager::LoadExtension() #Error", i_ExtensionPath.string().c_str() );
		}
		
		// Load the shared object
		void* Extension = dlopen( i_ExtensionPath.string().c_str(), RTLD_NOW | RTLD_LOCAL );
		
		syslog( LOG_DEBUG, "loading extension %s #ExtensionManager::LoadExtension() #Debug", i_ExtensionPath.string().c_str() );
		
		if( !Extension ) {
			throw std::runtime_error( dlerror() );
		}
		
		// Look for the "attach" symbol that returns the factory object
		void* AttachSymbol = dlsym( Extension, "Attach" );
		
		if( !AttachSymbol ) {
			throw std::runtime_error( dlerror() );
		}
		
		// Get our factory object
		ExtensionFactory* MyExtensionFactory = reinterpret_cast<ExtensionFactory*(*)(void)>( AttachSymbol )();
		
		// Push the entire extension (shared object, path, and factory object) to a container
		m_Extensions.push_back( std::tuple<void*, std::string, ExtensionFactory*>( Extension, i_ExtensionPath.string(), MyExtensionFactory ) );
		
		// Return newly-created factory object
		return MyExtensionFactory;
		
	} catch( std::exception& e ) {
		syslog( LOG_ERR, "%s #ExtensionManager::LoadExtension() #Error", e.what() );
	}
	
	return std::nullptr_t();
	
}
