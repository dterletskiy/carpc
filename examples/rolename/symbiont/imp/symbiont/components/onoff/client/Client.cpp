#include "api/sys/helpers/macros/strings.hpp"
// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



using namespace symbiont::components::onoff;



Client::Client( )
   : base::onoff::Client( "hybrid" )
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
   test( );
}

void Client::disconnected( )
{
   MSG_DBG( "disconnected" );
   unsubscribe_current_state( );
}

void Client::request_start( )
{
   MSG_DBG( );
   base::onoff::Client::request_start( );
}


void Client::response_trigger_state( const bool result )
{
   // MSG_DBG( "result: %s", BOOL_TO_STRING( result ) );
   test( );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   // MSG_DBG( "state: %s / delay: %zu", state.c_str( ), delay );
   base::onoff::Client::request_trigger_state( state, delay );
}

void Client::request_trigger_state_failed( const base::service::eError& error )
{
   MSG_ERR( "request error: %s", error.c_str( ) );
}

void Client::on_current_state( const std::string& state )
{
   MSG_DBG( "current state changed to '%s'", state.c_str( ) );
}

void Client::test( )
{
   static std::size_t count = 0;

   if( 0 == count )
   {
      MSG_DBG( "TEST STARTED CARPC" );
      m_performance.start( );
   }

   if( 1000000 > ++count )
   {
      request_trigger_state( "REQUEST_TEST", 0 );
   }
   else
   {
      m_performance.stop( );
      MSG_DBG( "TEST FINISHED CARPC: count = %zu", count );
   }
}
