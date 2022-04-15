// Application
#include "OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff_Server"



using namespace symbiont::interface::onoff;



Server::Server( )
   : service::onoff::V1_0_0::Server( "symbiont" )
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
