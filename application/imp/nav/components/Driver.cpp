// Application
#include "imp/nav/components/Driver.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Driver"



namespace application::driver {



base::ComponentPtr creator( base::ServicePtr p_service )
{
   return std::make_shared< Driver >( p_service, "Driver" );
}



Driver::Driver( const base::ServicePtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::PingDriverEvent::Event::set_notification( true, this );
   events::PingMasterEvent::Event::set_notification( true, this );
}

Driver::~Driver( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::PingDriverEvent::Event::set_notification( false, this );
   events::PingMasterEvent::Event::set_notification( false, this );
}

void Driver::process_event( const events::PingDriverEvent::Event& event )
{
   DBG_TRC( "type = %#zx, info = %s", static_cast< size_t >( event.data( )->type ), event.data( )->info.c_str( ) );
   switch( event.data( )->type )
   {
      case events::ePing::ping:
      {
         events::PingMasterEvent::Event::send_event( { events::ePing::ping, "Driver -> Master" }, base::eCommType::ITC );
         break;
      }
      case events::ePing::response:
      {
         break;
      }
      default:
      {
         break;
      }
   }
}

void Driver::process_event( const events::PingMasterEvent::Event& event )
{
   DBG_TRC( "type = %#zx, info = %s", static_cast< size_t >( event.data( )->type ), event.data( )->info.c_str( ) );
   switch( event.data( )->type )
   {
      case events::ePing::ping:
      {
         break;
      }
      case events::ePing::response:
      {
         events::PingDriverEvent::Event::send_event( { events::ePing::response, "OnOff <- Driver" }, base::eCommType::ITC );
         break;
      }
      default:
      {
         break;
      }
   }
}


} // namespace application::driver
