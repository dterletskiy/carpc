// Application
#include "imp/nav/components/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace application::onoff {



base::ComponentPtr creator( base::ServicePtr p_service )
{
   return std::make_shared< OnOff >( p_service, "OnOff" );
}



OnOff::OnOff( const base::ServicePtr p_service, const std::string& name )
   : base::RootComponent( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::PingDriverEvent::set_notification( true, this );
}

OnOff::~OnOff( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::PingDriverEvent::set_notification( false, this );
}

bool OnOff::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   events::PingDriverEvent::send_event( { events::ePing::ping, "OnOff -> Driver" }, base::eCommType::ITC );
   return true;
}

void OnOff::process_event( const events::PingDriverEvent& event )
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
         DBG_MSG( "System status OK" );
         exit( );
         break;
      }
      default:
      {
         break;
      }
   }
}


} // namespace application::onoff
