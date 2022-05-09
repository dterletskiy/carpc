#include "IClient.hpp"



using namespace service::onoff::V2_0_0;



IClient::IClient( const std::string& role_name )
   : carpc::service::experimental::TClient< data::Types >( role_name, true )
{
}

void IClient::request_start( )
{
   request< data::method::request::StartData >( this );
}

const carpc::comm::sequence::ID IClient::request_trigger_state( const std::string& state, const std::size_t delay )
{
   return request< data::method::request::TriggerStateData >( this, state, delay );
}

void IClient::subscribe_current_state( )
{
   subscribe< data::attribute::notification::CurrentStateData >( this );
}

void IClient::unsubscribe_current_state( )
{
   unsubscribe< data::attribute::notification::CurrentStateData >( this );
}

void IClient::process_event( const tMethod::Event& event )
{
   onoff::V2_0_0::method::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case onoff::V2_0_0::method::eID::TriggerState:
      {
         if( carpc::service::eType::RESPONSE == event_type )
         {
            using tMethodData = data::method::response::TriggerStateData;

            if( const tMethodData* p_event_data = get_event_data< tMethodData >( event ) )
               response_trigger_state( p_event_data->result );
            else
               request_trigger_state_failed( carpc::service::eError::EmptyData );
         }
         else if( carpc::service::eType::BUSY == event_type )
         {
            request_trigger_state_failed( carpc::service::eError::RequestBusy );
         }
         break;
      }
      default: break;
   }
}

void IClient::process_event( const tAttribute::Event& event )
{
   onoff::V2_0_0::attribute::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case onoff::V2_0_0::attribute::eID::CurrentState:
      {
         if( carpc::service::eType::NOTIFICATION == event_type )
         {
            using tAttributeData = data::attribute::notification::CurrentStateData;

            if( const tAttributeData* p_event_data = get_event_data< tAttributeData >( event ) )
               on_current_state( p_event_data->value );
         }
         break;
      }
      default: break;
   }
}
