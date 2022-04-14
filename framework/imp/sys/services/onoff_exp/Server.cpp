#include "api/sys/services/onoff_exp/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



using namespace service::onoff;



Server::Server( const std::string& role_name )
   : carpc::service::experimental::TServer< data::Types >( role_name, true )
{
}

Server::~Server( )
{
}

void Server::connected( )
{
   SYS_VRB( );
}

void Server::disconnected( )
{
   SYS_VRB( );
}

void Server::response_trigger_state( const bool result )
{
   SYS_DBG( "result: %s", BOOL_TO_STRING( result ) );

   response< data::method::response::TriggerStateData >( result );
}

void Server::process_request_event( const tMethod::Event& event )
{
   onoff::method::eID event_id = event.info( ).id( );
   carpc::service::eType event_type = event.info( ).type( );

   switch( event_id )
   {
      case onoff::method::eID::TriggerState:
      {
         if( carpc::service::eType::REQUEST == event_type )
         {
            using tMethodData = data::method::request::TriggerStateData;

            if( const tMethodData* p_event_data = get_event_data< tMethodData >( event ) )
               request_trigger_state( p_event_data->state, p_event_data->delay );
            else
               MSG_ERR( "Request '%s' data event empty", event_id.c_str( ) );
         }
         break;
      }
      default: break;
   }
}

void Server::current_state( const std::string& _current_state )
{
   notify< data::attribute::notification::CurrentStateData >( _current_state );
}

const std::string& Server::current_state( ) const
{
   if( auto p_data = attribute< data::attribute::notification::CurrentStateData >( ) )
      return p_data->state;

   static const std::string state = "";
   return state;
}
