#include "IServer.hpp"

using namespace service::onoff::V2_0_0;

IServer::IServer( const std::string& role_name )
   : carpc::service::experimental::TServer< Types >( role_name, true )
{
}

void IServer::response_trigger_state( const bool& result )
{
   response< method::response::trigger_stateData >( result );
}

void IServer::process_request_event( const tMethod::Event& event )
{
   method::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   if( carpc::service::eType::REQUEST != event_type )
      return;

   switch( event_id )
   {
      case method::eID::trigger_state:
      {
         using tMethodData = method::request::trigger_stateData;
         if( const tMethodData* p_event_data = get_event_data< tMethodData >( event ) )
            request_trigger_state( p_event_data->state, p_event_data->timeout );
         break;
      }
      case method::eID::start:
      {
         using tMethodData = method::request::startData;
         if( const tMethodData* p_event_data = get_event_data< tMethodData >( event ) )
            request_start( );
         break;
      }
      default: break;
   }
}

void IServer::current_state( const std::string& value )
{
   notify< attribute::notification::current_stateData >( value );
}

const std::string& IServer::current_state( ) const
{
   if( auto p_data = attribute< attribute::notification::current_stateData >( ) )
      return p_data->value;

   static const std::string dummy{ };
   return dummy;
}


