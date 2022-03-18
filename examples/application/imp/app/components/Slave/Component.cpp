// Application
#include "imp/app/components/Slave/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Slave"



using namespace application::components::slave;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "Slave" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::Component( _name )
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
            // auto request = [ ]( ){  };
            auto request = [ this ]( ){ mp_client_onoff->request_trigger_state( "Slave", 1000000000 ); };
            auto request_start = [ this ]( ){ mp_client_onoff->request_start( ); };
            mp_client_onoff = std::shared_ptr< clients::onoff::Client >(
               new clients::onoff::Client( "OnOffService", "OnOffService-Client-Slave", request )
            );
         }
         break;
      }
      default: break;
   }
}
