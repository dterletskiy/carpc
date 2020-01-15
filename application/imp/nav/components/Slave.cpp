// Application
#include "imp/nav/components/Slave.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Slave"



namespace application::slave {



base::ComponentPtr creator( base::ServicePtr p_service )
{
   return std::make_shared< Slave >( p_service, "Slave" );
}



Slave::Slave( const base::ServicePtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::PingSlaveEvent::set_notification( true, this );
}

Slave::~Slave( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::PingSlaveEvent::set_notification( false, this );
}

void Slave::process_event( const events::PingSlaveEvent& event )
{
   // DBG_TRC( "type = %#x, info = %s", event.data( )->type, event.data( )->info.c_str( ) );
   switch( event.data( )->type )
   {
      case events::ePing::ping:
      {
         DBG_MSG( "type = %#x, info = %s", event.data( )->type, event.data( )->info.c_str( ) );
         sleep( 15 );
         events::PingSlaveEvent::send_event( { events::ePing::response, "Master <- Slave" }, base::eCommType::ETC );
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


} // namespace application::slave
