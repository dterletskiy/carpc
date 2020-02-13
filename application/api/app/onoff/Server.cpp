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
   DBG_MSG( );
}

void Server::disconnected( )
{
   DBG_MSG( );
}

void Server::response_trigger_state( const bool result )
{
   DBG_TRC( "result: %s", BOOL_TO_STRING( result ) );

   create_send_response< data::ResponseTriggerStateData >( result );
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



} // namespace api::onoff
