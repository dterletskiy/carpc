#include "api/sys/services/onoff/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



using namespace carpc::onoff;



Server::Server( const std::string& role_name )
   : carpc::service::SERVICE_NAMESPACE::TServer< data::Types >( role_name, true )
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

   response< data::ResponseTriggerStateData >( result );
}

void Server::process_request_event( const tService::Event& event )
{
   switch( event.info( ).id( ) )
   {
      case eOnOff::RequestStart:
      {
         request_start( );
         break;
      }
      case eOnOff::RequestTriggerState:
      {
         if( const data::RequestTriggerStateData* request_data = get_event_data< data::RequestTriggerStateData >( event ) )
         {
            request_trigger_state( request_data->state, request_data->delay );
         }
         break;
      }
      default: break;
   }
}

void Server::current_state( const std::string& _current_state )
{
   notify< data::NotificationCurrentStateData >( _current_state );
}

const std::string& Server::current_state( ) const
{
   if( auto p_data = attribute< data::NotificationCurrentStateData >( ) )
      return p_data->state;

   static const std::string state = "";
   return state;
}
