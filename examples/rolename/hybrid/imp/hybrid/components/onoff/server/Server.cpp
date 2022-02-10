// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServer"



using namespace hybrid::components::onoff;



Server::Server( )
   : base::onoff::Server( "hybrid" )
{
   MSG_DBG( "created" );
   current_state( "UNLOADED" );
}

Server::~Server( )
{
   MSG_DBG( "destroyed" );
}

void Server::connected( )
{
   MSG_DBG( "connected" );
}

void Server::disconnected( )
{
   MSG_DBG( "disconnected" );
}

void Server::request_start( )
{
   MSG_DBG( );
}

void Server::request_trigger_state( const std::string& state, const size_t delay )
{
   MSG_DBG( "state: %s / delay: %zu", state.c_str( ), delay );
   response_trigger_state( true );
   current_state( state );
}
