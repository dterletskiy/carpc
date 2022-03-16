#include "api/sys/services/onoff/Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



using namespace base::onoff;



Client::Client( const std::string& role_name )
   : base::service::SERVICE_NAMESPACE::TClient< data::Types >( role_name, true )
{
}

Client::~Client( )
{
}

void Client::connected( )
{
   SYS_DBG( );
}

void Client::disconnected( )
{
   SYS_DBG( );
}

void Client::request_start( )
{
   SYS_VRB( );
   request< data::RequestStartData >( this );
}

const base::comm::sequence::ID Client::request_trigger_state( const std::string& state, const std::size_t delay )
{
   SYS_VRB( "state: %s / delay: %zu", state.c_str( ), delay );
   return request< data::RequestTriggerStateData >( this, state, delay );
}

void Client::subscribe_current_state( )
{
   SYS_VRB( );
   subscribe< data::NotificationCurrentStateData >( this );
}

void Client::unsubscribe_current_state( )
{
   SYS_VRB( );
   unsubscribe< data::NotificationCurrentStateData >( this );
}

void Client::process_response_event( const tService::Event& event )
{
   switch( event.info( ).id( ) )
   {
      case eOnOff::ResponseTriggerState:
      {
         if( const data::ResponseTriggerStateData* data = get_event_data< data::ResponseTriggerStateData >( event ) )
         {
            response_trigger_state( data->result );
         }
         else
         {
            request_trigger_state_failed( base::service::eError::EmptyData );
         }
         break;
      }
      case eOnOff::RequestTriggerStateBusy:
      {
         request_trigger_state_failed( base::service::eError::RequestBusy );
         break;
      }
      default: break;
   }
}

void Client::process_notification_event( const tService::Event& event )
{
   switch( event.info( ).id( ) )
   {
      case eOnOff::NotificationCurrentState:
      {
         if( const data::NotificationCurrentStateData* data = get_event_data< data::NotificationCurrentStateData >( event ) )
         {
            on_current_state( data->state );
         }
         break;
      }
      default: break;
   }
}
