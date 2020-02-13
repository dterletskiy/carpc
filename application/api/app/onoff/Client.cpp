// Framework
// Application
#include "Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



namespace api::onoff {



Client::Client( const std::string& role_name )
   : base::TClient< data::Types >( )
{
   // DBG_TRC( "Created" );
   mp_proxy = base::TProxy< data::Types >::create( api::onoff::interface_name, role_name );
   mp_proxy->register_client( this );
}

Client::~Client( )
{
   // DBG_TRC( "Destroyed" );
   mp_proxy->unregister_client( this );
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

   return mp_proxy->create_send_request< data::RequestTriggerStateData >( this, state, delay );
}

void Client::process_response_event( const data::OnOffEvent::Event& event )
{
   switch( event.info( ).id( ) )
   {
      case eOnOff::ResponseTriggerState:
      {
         const data::ResponseTriggerStateData* data = mp_proxy->get_event_data< data::ResponseTriggerStateData >( event );
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
