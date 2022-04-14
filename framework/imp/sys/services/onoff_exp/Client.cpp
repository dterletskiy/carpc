#include "api/sys/services/onoff_exp/Client.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClientBase"



using namespace service::onoff;



Client::Client( const std::string& role_name )
   : carpc::service::experimental::TClient< data::Types >( role_name, true )
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
   request< data::method::request::StartData >( this );
}

const carpc::comm::sequence::ID Client::request_trigger_state( const std::string& state, const std::size_t delay )
{
   SYS_VRB( "state: %s / delay: %zu", state.c_str( ), delay );
   return request< data::method::request::TriggerStateData >( this, state, delay );
}

void Client::subscribe_current_state( )
{
   SYS_VRB( );
   subscribe< data::attribute::notification::CurrentStateData >( this );
}

void Client::unsubscribe_current_state( )
{
   SYS_VRB( );
   unsubscribe< data::attribute::notification::CurrentStateData >( this );
}

void Client::process_event( const tMethod::Event& event )
{
   onoff::method::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case onoff::method::eID::TriggerState:
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

void Client::process_event( const tAttribute::Event& event )
{
   onoff::attribute::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case onoff::attribute::eID::CurrentState:
      {
         if( carpc::service::eType::NOTIFICATION == event_type )
         {
            using tAttributeData = data::attribute::notification::CurrentStateData;

            if( const tAttributeData* p_event_data = get_event_data< tAttributeData >( event ) )
               on_current_state( p_event_data->state );
         }
         break;
      }
      default: break;
   }
}
