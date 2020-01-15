// Application
#include "imp/app/components/server/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServer"



namespace application::onoff {



Server::Server( )
{
   // DBG_MSG( "Created" );
}

Server::~Server( )
{
   // DBG_MSG( "Destroyed" );
}

void Server::request_trigger_state( const std::string& state )
{
   DBG_TRC( "state: %s", state.c_str( ) );
   response_trigger_state( true );
}



} // namespace application::onoff
