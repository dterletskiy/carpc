// Application
#include "imp/app/components/client/Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



namespace application::onoff {



Client::Client( )
{
   DBG_MSG( "Created" );
}

Client::~Client( )
{
   DBG_MSG( "Destroyed" );
}

void Client::response_trigger_state( const bool result )
{
   DBG_MSG( "result: %s", BOOL_TO_STRING( result ) );
}



} // namespace application::onoff
