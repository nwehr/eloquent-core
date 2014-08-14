#ifndef _ExtensionFactory_h
#define _ExtensionFactory_h

//
// Copyright 2013-2014 EvriChart, Inc. All Rights Reserved.
// See LICENSE.txt
//

// C++
#include <string>
#include <vector>

namespace Eloquent {
	const std::string ELOQUENT_EXTENSION_READER	= "read";
	const std::string ELOQUENT_EXTENSION_WRITER	= "write";
	const std::string ELOQUENT_EXTENSION_FILTER = "filter";
	
	///////////////////////////////////////////////////////////////////////////////
	// ExtensionFactory
	///////////////////////////////////////////////////////////////////////////////
	class ExtensionFactory {
	public:
		explicit ExtensionFactory( const std::string& i_Name
								  , const std::string& i_Version
								  , const std::string& i_Author
								  , const std::string& i_Type
								  , const std::vector<std::string>& i_Keys );

		virtual ~ExtensionFactory();

		const std::string& Name() const;
		const std::string& Version() const;
		const std::string& Author() const;
		const std::string& Type() const;
		const std::vector<std::string>& Keys() const;
		
	protected:
		std::string m_Name;
		std::string m_Version;
		std::string m_Author;
		std::string m_Type;
		std::vector<std::string> m_Keys;
		
	};

} // namespace Eloquent

#endif // _ExtensionFactory_h
