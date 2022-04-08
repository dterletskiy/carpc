#include "api/sys/helpers/macros/strings.hpp"
// Application
#include "OnOff_v.1.0.0.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff_Client_OnOff_1.0.0"



using namespace application::components::onoff::client::V1_0_0;



OnOff::OnOff( const std::string& role_name )
   : service::onoff::V1_0_0::Client( role_name )
{
   MSG_VRB( "created" );
}

OnOff::~OnOff( )
{
   MSG_VRB( "destroyed" );
   unsubscribe_current_state( );
}

void OnOff::connected( )
{
   MSG_DBG( );
   subscribe_current_state( );
   request_trigger_state( "BASIC_OPERABLE", 3 );
}

void OnOff::disconnected( )
{
   MSG_DBG( );
   unsubscribe_current_state( );
}

void OnOff::response_trigger_state( const bool result )
{
   MSG_DBG( "result: %s", BOOL_TO_STRING( result ) );
}

void OnOff::request_trigger_state( const std::string& state, const size_t delay )
{
   MSG_DBG( "state: %s / delay: %zu", state.c_str( ), delay );
   service::onoff::V1_0_0::Client::request_trigger_state( state, delay );
}

void OnOff::request_trigger_state_failed( const carpc::service::eError& error )
{
   MSG_ERR( "request error: %s", error.c_str( ) );
}

void OnOff::on_current_state( const std::string& state )
{
   MSG_DBG( "current state changed to '%s'", state.c_str( ) );
}
