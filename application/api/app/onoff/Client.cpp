// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



namespace api::onoff {



Client::Client( const std::string& role_name )
   : base::TClient< data::Types >( api::onoff::interface_name, role_name )
{
   // DBG_TRC( "Created" );
}

Client::~Client( )
{
   // DBG_TRC( "Destroyed" );
}

void Client::connected( )
{
   DBG_MSG( );
}

void Client::disconnected( )
{
   DBG_MSG( );
}

const size_t Client::request_trigger_state( const std::string& state, const size_t delay )
{
   DBG_TRC( "state: %s / delay: %zu", state.c_str( ), delay );

   return create_send_request< data::RequestTriggerStateData >( state, delay );
}

void Client::process_response_event( const data::OnOffEvent::Event& event )
{
   switch( event.info( ).id( ) )
   {
      case eOnOff::ResponseTriggerState:
      {
         const data::ResponseTriggerStateData* data = get_event_data< data::ResponseTriggerStateData >( event );
         response_trigger_state( data->result );
         break;
      }
      case eOnOff::RequestTriggerStateBusy:
      {
         request_trigger_state_failed( );
      }
      default: break;
   }
}



} // namespace api::onoff
