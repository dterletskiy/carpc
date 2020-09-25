// Application
#include "imp/app/components/Slave/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Slave"



namespace application::components::slave {



base::IComponent::tSptr Component::creator( base::IServiceThread& service )
{
   return std::shared_ptr< Component >( new Component( service, "Slave" ) );
}

Component::Component( base::IServiceThread& service, const std::string& name )
   : base::Component( service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::BOOT );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::SHUTDOWN );
   events::AppEvent::Event::set_notification( this, events::eAppEventID::PING );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::AppEvent::Event::clear_all_notifications( this );
}

void Component::process_event( const events::AppEvent::Event& event )
{
   DBG_MSG( "message = %s", event.data( )->message.c_str( ) );

   switch( event.info( ).id( ) )
   {
      case events::eAppEventID::BOOT:
      {
         if( nullptr == mp_client_onoff )
         {
            // auto request = [ ]( ){  };
            auto request = [ this ]( ){ mp_client_onoff->request_trigger_state( "Slave", 1000000000 ); };
            auto request_start = [ this ]( ){ mp_client_onoff->request_start( ); };
            mp_client_onoff = new clients::onoff::Client( "OnOffService", "OnOffService-Client-Slave", request );
         }
         break;
      }
      default: break;
   }
}



} // namespace application::components::slave
