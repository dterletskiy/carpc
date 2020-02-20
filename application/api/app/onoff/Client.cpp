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
   mp_proxy->subscribe< data::NotificationCurrentStateData >( this );
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
   return mp_proxy->request< data::RequestTriggerStateData >( this, state, delay );
}

void Client::subscribe_current_state( )
{
   DBG_TRC( );
   mp_proxy->subscribe< data::NotificationCurrentStateData >( this );
}

void Client::unsubscribe_current_state( )
{
   DBG_TRC( );
   mp_proxy->unsubscribe< data::NotificationCurrentStateData >( this );
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

void Client::process_notification_event( const data::OnOffEvent::Event& event )
{
   switch( event.info( ).id( ) )
   {
      case eOnOff::NotificationCurrentState:
      {
         const data::NotificationCurrentStateData* data = mp_proxy->get_event_data< data::NotificationCurrentStateData >( event );
         on_current_state( data->state );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff
