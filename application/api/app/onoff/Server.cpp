// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



namespace api::onoff {



Server::Server( const std::string& role_name )
   : base::TServer< data::Types >( api::onoff::interface_name, role_name )
{
   // DBG_TRC( "Created" );
}

Server::~Server( )
{
   // DBG_TRC( "Destroyed" );
}

void Server::connected( )
{
   DBG_TRC( );
}

void Server::disconnected( )
{
   DBG_TRC( );
}

void Server::response_trigger_state( const bool result )
{
   DBG_MSG( "result: %s", BOOL_TO_STRING( result ) );

   response< data::ResponseTriggerStateData >( result );
}

void Server::process_request_event( const data::OnOffEvent::Event& event )
{
   switch( event.info( ).id( ) )
   {
      case eOnOff::RequestTriggerState:
      {
         const data::RequestTriggerStateData* request_data = get_event_data< data::RequestTriggerStateData >( event );
         request_trigger_state( request_data->state, request_data->delay );
         break;
      }
      default: break;
   }
}

void Server::current_state( const std::string& _current_state )
{
   DBG_MSG( );
   m_current_state = _current_state;
   notify< data::NotificationCurrentStateData >( m_current_state );
}

const std::string& Server::current_state( ) const
{
   DBG_MSG( );
   return m_current_state;
}



} // namespace api::onoff
