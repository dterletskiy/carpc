#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/comm/timer/Timer.hpp"
// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



using namespace hmi::components::onoff;



Client::Client( )
   : service::onoff::V1_0_0::Client( "OnOff_Controller" )
{
   MSG_VRB( "created" );
}

Client::~Client( )
{
   MSG_VRB( "destroyed" );
   unsubscribe_current_state( );
}

void Client::connected( )
{
   MSG_DBG( "connected" );
   // subscribe_current_state( );
   // request_trigger_state( "BASIC OPERABLE", 10000000000 );

   carpc::timer::start(
      10000, 1,
      [ this ]( const carpc::comm::timer::ID id )
      {
         MSG_DBG( "Timer expired" );
         subscribe_current_state( );
         request_trigger_state( "BASIC OPERABLE", 10000000000 );
      }
   );
}

void Client::disconnected( )
{
   MSG_DBG( "disconnected" );
   unsubscribe_current_state( );
}

void Client::request_start( )
{
   MSG_DBG( );
   service::onoff::V1_0_0::Client::request_start( );
}


void Client::response_trigger_state( const bool result )
{
   MSG_DBG( "result: %s", BOOL_TO_STRING( result ) );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   MSG_DBG( "state: %s / delay: %zu", state.c_str( ), delay );
   service::onoff::V1_0_0::Client::request_trigger_state( state, delay );
}

void Client::request_trigger_state_failed( const carpc::service::eError& error )
{
   MSG_ERR( "request error: %s", error.c_str( ) );
}

void Client::on_current_state( const std::string& state )
{
   MSG_DBG( "current state changed to '%s'", state.c_str( ) );
}
