// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



namespace api::onoff {



Server::Server( const std::string& role_name )
   : base::Server( api::onoff::interface_name, role_name, api::onoff::is_ipc )
{
   // DBG_TRC( "Created" );
   set_notification< data::RequestTriggerStateData >( this );
}

Server::~Server( )
{
   // DBG_TRC( "Destroyed" );
   clear_all_notifications< data::BaseData >( this );
}

void Server::connected( ) const
{
   DBG_MSG( );
}

void Server::disconnected( ) const
{
   DBG_MSG( );
}

void Server::response_trigger_state( const bool result )
{
   DBG_TRC( "result: %s", BOOL_TO_STRING( result ) );

   create_send< data::ResponseTriggerStateData >( result );
   busy( false );
}

void Server::process_event( const data::OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.info( ).id( ) ).c_str( ) );

   switch( event.info( ).id( ) )
   {
      case eOnOff::RequestTriggerState:
      {
         if( busy( true ) )
         {
            DBG_WRN( "request busy" );
            return;
         }
         const data::RequestTriggerStateData* data = get_event_data< data::RequestTriggerStateData >( event );
         request_trigger_state( data->state, data->delay );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff
