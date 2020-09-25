#include "api/sys/onoff/Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



using namespace base::onoff;



Server::Server( const std::string& role_name )
   : base::interface::TServer< data::Types >( base::onoff::interface_type_id, role_name, true )
{
   SYS_TRC( "Created" );
}

Server::~Server( )
{
   SYS_TRC( "Destroyed" );
}

void Server::connected( )
{
   SYS_TRC( );
}

void Server::disconnected( )
{
   SYS_TRC( );
}

void Server::response_trigger_state( const bool result )
{
   SYS_MSG( "result: %s", BOOL_TO_STRING( result ) );

   response< data::ResponseTriggerStateData >( result );
}

void Server::process_request_event( const data::OnOffEvent::Event& event )
{
   auto event_id = event.info( ).id( );
   switch( event_id )
   {
      case eOnOff::RequestStart:
      {
         request_start( );
         break;
      }
      case eOnOff::RequestTriggerState:
      {
         const data::RequestTriggerStateData* request_data = get_event_data< data::RequestTriggerStateData >( event );
         if( nullptr == request_data )
         {
            SYS_ERR( "missing tequest data for request ID: %s", to_string( event_id ).c_str( ) );
         }
         else
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
   auto p_data = attribute< data::NotificationCurrentStateData >( );
   if( nullptr == p_data )
   {
      static const std::string state = "";
      return state;
   }
   return p_data->state;
}
