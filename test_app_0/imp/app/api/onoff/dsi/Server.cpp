// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



namespace api::onoff::dsi {



Server::Server( const base::ServiceName& service_name )
   : m_service_name( service_name )
{
   // DBG_TRC( "Created" );
   OnOffEvent::Event::set_notification( this, m_service_name, eOnOff::RequestTriggerState );
}

Server::~Server( )
{
   // DBG_TRC( "Destroyed" );
   OnOffEvent::Event::clear_all_notifications( this );
}

void Server::response_trigger_state( const bool result )
{
   DBG_TRC( "result: %s", BOOL_TO_STRING( result ) );

   OnOffEvent::Data data( std::make_shared< ResponseTriggerStateData >( result ) );
   OnOffEvent::Event::create_send(
           m_service_name
         , eOnOff::ResponseTriggerState
         , data
         , base::eCommType::IPC
      );
}

void Server::process_event( const OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.info( ).id( ) ).c_str( ) );
   switch( event.info( ).id( ) )
   {
      case eOnOff::RequestTriggerState:
      {
         RequestTriggerStateData* data = static_cast< RequestTriggerStateData* >( event.data( )->ptr.get( ) );
         request_trigger_state( data->state, data->delay );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff::dsi
