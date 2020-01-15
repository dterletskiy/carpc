// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServerBase"



namespace api::onoff::no_dsi {



Server::Server( )
{
   // DBG_TRC( "Created" );
   OnOffEvent::Event::set_notification( true, this, eOnOff::RequestTriggerState );
}

Server::~Server( )
{
   // DBG_TRC( "Destroyed" );
   OnOffEvent::Event::set_notification( false, this, eOnOff::RequestTriggerState );
}

void Server::response_trigger_state( const bool result )
{
   DBG_TRC( "result: %s", BOOL_TO_STRING( result ) );

   OnOffEvent::Data data( std::make_shared< ResponseTriggerStateData >( result ) );
   OnOffEvent::Event::create_send(
           data
         , eOnOff::ResponseTriggerState
         , base::eCommType::ITC
      );
}

void Server::process_event( const OnOffEvent::Event& event )
{
   DBG_TRC( "id = %s", to_string( event.id( ) ).c_str( ) );
   switch( event.id( ) )
   {
      case eOnOff::RequestTriggerState:
      {
         RequestTriggerStateData* data = static_cast< RequestTriggerStateData* >( event.data( )->ptr.get( ) );
         request_trigger_state( data->state );
         break;
      }
      default: break;
   }
}



} // namespace api::onoff::no_dsi
