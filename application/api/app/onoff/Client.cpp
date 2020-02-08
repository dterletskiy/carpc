// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



namespace api::onoff {



Client::Client( const std::string& role_name )
   : base::Client( api::onoff::interface_name, role_name, api::onoff::is_ipc )
{
   // DBG_TRC( "Created" );
}

Client::~Client( )
{
   // DBG_TRC( "Destroyed" );
   clear_all_notifications< data::BaseData >( this );
}

void Client::connected( ) const
{
   DBG_MSG( );
}

void Client::disconnected( ) const
{
   DBG_MSG( );
}

void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_TRC( "state: %s", state.c_str( ) );

   set_notification< data::ResponseTriggerStateData >( this );

   create_send< data::RequestTriggerStateData >( state, delay );
}

void Client::process_event( const data::OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.info( ).id( ) ).c_str( ) );
   switch( event.info( ).id( ) )
   {
      case eOnOff::ResponseTriggerState:
      {
         clear_notification< data::ResponseTriggerStateData >( this );

         const data::ResponseTriggerStateData* data = get_event_data< data::ResponseTriggerStateData >( event );
         response_trigger_state( data->result );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff
