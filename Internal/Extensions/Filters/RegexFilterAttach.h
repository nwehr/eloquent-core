#ifndef __eloquent__RegexFilterAttach__
#define __eloquent__RegexFilterAttach__

// Boost
#include <boost/smart_ptr.hpp>

// Internal
#include "RegexFilter.h"
#include "RegexFilterFactory.h"

///////////////////////////////////////////////////////////////////////////////
// Library Initializer
///////////////////////////////////////////////////////////////////////////////
extern "C" boost::shared_ptr<Eloquent::ExtensionFactory> Attach( void );

#endif /* defined(__eloquent__RegexFilterAttach__) */