#include "api/sys/helpers/macros/strings.hpp"
// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



namespace application::clients::onoff {



Client::Client( const std::string& role_name, const std::string& name, tConnectedCallback connected_callback )
   : api::onoff::Client( role_name )
   , m_name( name )
   , m_connected_callback( connected_callback )
{
   DBG_TRC( "%s: created", m_name.c_str( ) );
   subscribe_current_state( );
}

Client::~Client( )
{
   DBG_TRC( "%s: destroyed", m_name.c_str( ) );
   unsubscribe_current_state( );
}

void Client::connected( )
{
   DBG_MSG( );
   m_connected_callback( );
}

void Client::disconnected( )
{
   DBG_MSG( );
}

void Client::request_start( )
{
   DBG_MSG( "%s", m_name.c_str( ) );
   api::onoff::Client::request_start( );
}


void Client::response_trigger_state( const bool result )
{
   DBG_MSG( "%s: result: %s", m_name.c_str( ), BOOL_TO_STRING( result ) );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_MSG( "%s: state: %s / delay: %zu", m_name.c_str( ), state.c_str( ), delay );
   api::onoff::Client::request_trigger_state( state, delay );
}

void Client::request_trigger_state_failed( )
{
   DBG_ERR( "%s: request failed", m_name.c_str( ) );
}

void Client::on_current_state( const std::string& state )
{
   DBG_MSG( "%s: current state changed to '%s'", m_name.c_str( ), state.c_str( ) );
}



} // namespace application::clients::onoff
