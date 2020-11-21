#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/comm/timer/Timer.hpp"
// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



using namespace hmi::components::onoff;



Client::Client( )
   : base::onoff::Client( "OnOff_Controller" )
{
   DBG_TRC( "created" );
}

Client::~Client( )
{
   DBG_TRC( "destroyed" );
   unsubscribe_current_state( );
}

void Client::connected( )
{
   DBG_MSG( "connected" );
   // subscribe_current_state( );
   // request_trigger_state( "BASIC OPERABLE", 10000000000 );

   base::timer::start(
      10000, 1,
      [ this ]( const base::Timer::ID id )
      {
         DBG_MSG( "Timer expired" );
         subscribe_current_state( );
         request_trigger_state( "BASIC OPERABLE", 10000000000 );
      }
   );
}

void Client::disconnected( )
{
   DBG_MSG( "disconnected" );
   unsubscribe_current_state( );
}

void Client::request_start( )
{
   DBG_MSG( );
   base::onoff::Client::request_start( );
}


void Client::response_trigger_state( const bool result )
{
   DBG_MSG( "result: %s", BOOL_TO_STRING( result ) );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_MSG( "state: %s / delay: %zu", state.c_str( ), delay );
   base::onoff::Client::request_trigger_state( state, delay );
}

void Client::request_trigger_state_failed( )
{
   DBG_ERR( "request failed" );
}

void Client::on_current_state( const std::string& state )
{
   DBG_MSG( "current state changed to '%s'", state.c_str( ) );
}
