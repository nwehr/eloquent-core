// C
#include <dlfcn.h>

// Internal
#include "ExtensionManager.h"
#include "Logging.h"

Eloquent::ExtensionManager* Eloquent::ExtensionManager::m_Instance;

Eloquent::ExtensionManager::ExtensionManager( std::mutex& i_LogMutex, streamlog::severity_log& i_Log )
: m_LogMutex( i_LogMutex )
, m_Log( i_Log )
{}

Eloquent::ExtensionManager::~ExtensionManager() {
	for( std::tuple<void*, std::string, ExtensionFactory*>& Extension : m_Extensions ) {
		delete std::get<2>( Extension );
		dlclose( std::get<0>( Extension ) );
		
	}
}
		
Eloquent::ExtensionManager& Eloquent::ExtensionManager::Instance( std::mutex& i_LogMutex, streamlog::severity_log& i_Log ) {
	if( !m_Instance )
		m_Instance = new Eloquent::ExtensionManager( i_LogMutex, i_Log );
	
	return *m_Instance;

}

Eloquent::ExtensionFactory* Eloquent::ExtensionManager::LoadExtension( const boost::filesystem::path& i_ExtensionPath ) {
	try {
		for( std::tuple<void*, std::string, ExtensionFactory*>& Extension : m_Extensions ) {
			if( i_ExtensionPath.string() == std::get<1>( Extension ) ) {
				return std::get<2>( Extension );
			}
			
		}
		
		boost::filesystem::path ExtensionPath = i_ExtensionPath;
		
		if( !boost::filesystem::exists( ExtensionPath ) ) {
			std::unique_lock<std::mutex> LogLock( m_LogMutex );
			m_Log( LogSeverity::SEV_ERROR ) << TimeAndSpace() << "unable to find extension at " << ExtensionPath.string() << " #Error #Filesystem #Core" << std::endl;
		}
		
		void* Extension = dlopen( ExtensionPath.string().c_str(), RTLD_LAZY | RTLD_LOCAL );
		
		if( !Extension ) {
			{
				std::unique_lock<std::mutex> LogLock( m_LogMutex );
				m_Log( LogSeverity::SEV_ERROR ) << TimeAndSpace() << "unable to load extension at " << ExtensionPath.string() << ". debug message following: #Error #Filesystem #Core" << std::endl;
				m_Log( LogSeverity::SEV_ERROR ) << std::string( dlerror() ) << std::endl;
				
			}
			
		} else {
			void* AttachSymbol = dlsym( Extension, "Attach" );
			
			if( !AttachSymbol ) {
				std::unique_lock<std::mutex> LogLock( m_LogMutex );
				m_Log( LogSeverity::SEV_ERROR ) << TimeAndSpace() << "unable to load extension. no attach symbol at " << ExtensionPath.string() << "#Error #Core" << std::endl;
			} else {
				{
					std::unique_lock<std::mutex> LogLock( m_LogMutex );
					m_Log( LogSeverity::SEV_DEBUG ) << TimeAndSpace() << "loading extension " << ExtensionPath.string() << " #Comment #Core" << std::endl;
				}
				
				ExtensionFactory* (*AttachFunction)(void) = 0;
				
				*reinterpret_cast<void**>( &AttachFunction ) = AttachSymbol;
				
				ExtensionFactory* MyExtensionFactory = AttachFunction();
				
				m_Extensions.push_back( std::tuple<void*, std::string, ExtensionFactory*>( Extension, ExtensionPath.string(), MyExtensionFactory ) );
				
				return MyExtensionFactory;
				
			}
			
		}
		
	} catch( std::exception& e ) {
		std::unique_lock<std::mutex> LogLock( m_LogMutex );
		m_Log( LogSeverity::SEV_ERROR ) << TimeAndSpace() << e.what() << " #Error #Core" << std::endl;
	}
	
	return std::nullptr_t();
	
}
