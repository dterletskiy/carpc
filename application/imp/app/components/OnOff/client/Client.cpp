#include "api/sys/helpers/macros/strings.hpp"
// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



namespace application::components::onoff {



Client::Client( const std::string& role_name, const std::string& name )
   : api::onoff::Client( role_name )
   , m_name( name )
{
   DBG_MSG( "%s: created", m_name.c_str( ) );
}

Client::~Client( )
{
   DBG_MSG( "%s: destroyed", m_name.c_str( ) );
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



} // namespace application::components::onoff
