#include "Driver.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "Driver"


namespace application::driver {



base::IComponentPtr creator( base::IServicePtr p_service )
{
   return std::make_shared< Driver >( p_service, "Driver" );
}



Driver::Driver( const base::IServicePtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::PingDriverEvent::set_notification( true, this );
   events::PingMasterEvent::set_notification( true, this );
}

Driver::~Driver( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::PingDriverEvent::set_notification( false, this );
   events::PingMasterEvent::set_notification( false, this );
}

void Driver::process_event( const events::PingDriverEvent& event )
{
   // DBG_TRC( "type = %#x, info = %s", event.data( )->type, event.data( )->info.c_str( ) );
   switch( event.data( )->type )
   {
      case events::ePing::ping:
      {
         DBG_MSG( "type = %#x, info = %s", event.data( )->type, event.data( )->info.c_str( ) );
         events::PingMasterEvent::send_event( { events::ePing::ping, "Driver -> Master" }, base::eCommType::ITC );
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

void Driver::process_event( const events::PingMasterEvent& event )
{
   // DBG_TRC( "type = %#x, info = %s", event.data( )->type, event.data( )->info.c_str( ) );
   switch( event.data( )->type )
   {
      case events::ePing::ping:
      {
         break;
      }
      case events::ePing::response:
      {
         DBG_MSG( "type = %#x, info = %s", event.data( )->type, event.data( )->info.c_str( ) );
         events::PingDriverEvent::send_event( { events::ePing::response, "OnOff <- Driver" }, base::eCommType::ITC );
         break;
      }
      default:
      {
         break;
      }
   }
}


} // namespace application::driver
