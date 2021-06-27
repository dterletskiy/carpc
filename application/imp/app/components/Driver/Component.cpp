// Application
#include "imp/app/components/Driver/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Driver"



using namespace application::components::driver;



base::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "Driver" ) );
}

Component::Component( const std::string& _name )
   : base::application::Component( _name )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::BOOT );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::SHUTDOWN );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::PING );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
   events::AppEvent::Event::clear_all_notifications( this );
}

void Component::process_event( const events::AppEvent::Event& event )
{
   MSG_DBG( "message = %s", event.data( )->message.c_str( ) );

   switch( event.info( ).id( ) )
   {
      case events::eAppEventID::BOOT:
      {
         if( nullptr == mp_client_onoff )
         {
            auto request = [ this ]( ){ mp_client_onoff->request_trigger_state( "Driver", 10000000000 ); };
            auto request_start = [ this ]( ){ mp_client_onoff->request_start( ); };
            mp_client_onoff = std::shared_ptr< clients::onoff::Client >(
               new clients::onoff::Client( "OnOffService", "OnOffService-Client-Driver", request )
            );
         }
         break;
      }
      default: break;
   }
}
