#include "IClient.hpp"



using namespace service::onoff::V1_0_0;



IClient::IClient( const std::string& role_name )
   : carpc::service::SERVICE_NAMESPACE::TClient< data::Types >( role_name, true )
{
}

void IClient::request_start( )
{
   request< data::RequestStartData >( this );
}

const carpc::comm::sequence::ID IClient::request_trigger_state( const std::string& state, const std::size_t delay )
{
   return request< data::RequestTriggerStateData >( this, state, delay );
}

void IClient::subscribe_current_state( )
{
   subscribe< data::NotificationCurrentStateData >( this );
}

void IClient::unsubscribe_current_state( )
{
   unsubscribe< data::NotificationCurrentStateData >( this );
}

void IClient::process_response_event( const tService::Event& event )
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
            request_trigger_state_failed( carpc::service::eError::EmptyData );
         }
         break;
      }
      case eOnOff::RequestTriggerStateBusy:
      {
         request_trigger_state_failed( carpc::service::eError::RequestBusy );
         break;
      }
      default: break;
   }
}

void IClient::process_notification_event( const tService::Event& event )
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
