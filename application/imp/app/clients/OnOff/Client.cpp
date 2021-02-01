#include "api/sys/helpers/macros/strings.hpp"
// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



using namespace application::clients::onoff;



Client::Client( const std::string& role_name, const std::string& name, tConnectedCallback connected_callback )
   : base::onoff::Client( role_name )
   , m_name( name )
   , m_connected_callback( connected_callback )
{
   MSG_VRB( "%s: created", m_name.c_str( ) );
}

Client::~Client( )
{
   MSG_VRB( "%s: destroyed", m_name.c_str( ) );
   unsubscribe_current_state( );
}

void Client::connected( )
{
   MSG_DBG( );
   m_connected_callback( );
   subscribe_current_state( );
}

void Client::disconnected( )
{
   MSG_DBG( );
   unsubscribe_current_state( );
}

void Client::request_start( )
{
   MSG_DBG( "%s", m_name.c_str( ) );
   base::onoff::Client::request_start( );
}


void Client::response_trigger_state( const bool result )
{
   MSG_DBG( "%s: result: %s", m_name.c_str( ), BOOL_TO_STRING( result ) );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   MSG_DBG( "%s: state: %s / delay: %zu", m_name.c_str( ), state.c_str( ), delay );
   base::onoff::Client::request_trigger_state( state, delay );
}

void Client::request_trigger_state_failed( )
{
   MSG_ERR( "%s: request failed", m_name.c_str( ) );
}

void Client::on_current_state( const std::string& state )
{
   MSG_DBG( "%s: current state changed to '%s'", m_name.c_str( ), state.c_str( ) );
}
