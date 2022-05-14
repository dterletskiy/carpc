#include "IClient.hpp"

using namespace service::onoff::V2_0_0;

IClient::IClient( const std::string& role_name )
   : carpc::service::experimental::TClient< Types >( role_name, true )
{
}

carpc::comm::sequence::ID IClient::request_trigger_state( const std::string& state, const std::size_t& timeout )
{
   return request< method::request::trigger_stateData >( this, state, timeout );
}

void IClient::request_start( )
{
   request< method::request::startData >( this );
}

void IClient::subscribe_current_state( )
{
   subscribe< attribute::notification::current_stateData >( this );
}

void IClient::unsubscribe_current_state( )
{
   unsubscribe< attribute::notification::current_stateData >( this );
}

void IClient::process_event( const tMethod::Event& event )
{
   method::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case method::eID::trigger_state:
      {
         if( carpc::service::eType::RESPONSE == event_type )
         {
            using tMethodData = method::response::trigger_stateData;
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
   attribute::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case attribute::eID::current_state:
      {
         if( carpc::service::eType::NOTIFICATION == event_type )
         {
            using tAttributeData = attribute::notification::current_stateData;
            if( const tAttributeData* p_event_data = get_event_data< tAttributeData >( event ) )
               on_current_state( p_event_data->value );
         }
         break;
      }
      default: break;
   }
}


