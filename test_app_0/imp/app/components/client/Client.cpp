// Application
#include "imp/app/components/client/Client.hpp"
#include "api/sys/helpers/macros/strings.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



namespace application::onoff {



Client::Client( const std::string& service_name )
   : api::onoff::no_dsi::Client( service_name )
{
   // DBG_TRC( "Created" );
}

Client::~Client( )
{
   // DBG_TRC( "Destroyed" );
}

void Client::response_trigger_state( const bool result )
{
   DBG_MSG( "result: %s", BOOL_TO_STRING( result ) );
}



} // namespace application::onoff
