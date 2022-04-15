#include "IServer.hpp"



using namespace service::onoff::V2_0_0;



IServer::IServer( const std::string& role_name )
   : carpc::service::experimental::TServer< data::Types >( role_name, true )
{
}

void IServer::response_trigger_state( const bool result )
{
   response< data::method::response::TriggerStateData >( result );
}

void IServer::process_request_event( const tMethod::Event& event )
{
   onoff::V2_0_0::method::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case onoff::V2_0_0::method::eID::TriggerState:
      {
         if( carpc::service::eType::REQUEST == event_type )
         {
            using tMethodData = data::method::request::TriggerStateData;

            if( const tMethodData* p_event_data = get_event_data< tMethodData >( event ) )
               request_trigger_state( p_event_data->state, p_event_data->delay );
            // else
            //    MSG_ERR( "Request '%s' data event empty", event_id.c_str( ) );
         }
         break;
      }
      default: break;
   }
}

void IServer::current_state( const std::string& _current_state )
{
   notify< data::attribute::notification::CurrentStateData >( _current_state );
}

const std::string& IServer::current_state( ) const
{
   if( auto p_data = attribute< data::attribute::notification::CurrentStateData >( ) )
      return p_data->state;

   static const std::string state = "";
   return state;
}
