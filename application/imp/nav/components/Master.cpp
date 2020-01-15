// Application
#include "imp/nav/components/Master.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Master"



namespace application::master {



base::ComponentPtr creator( base::ServicePtr p_service )
{
   return std::make_shared< Master >( p_service, "Master" );
}



Master::Master( const base::ServicePtr p_service, const std::string& name )
   : base::Component( p_service, name )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::PingMasterEvent::set_notification( true, this );
   events::PingSlaveEvent::set_notification( true, this );
}

Master::~Master( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::PingMasterEvent::set_notification( false, this );
   events::PingSlaveEvent::set_notification( false, this );
}

void Master::process_event( const events::PingMasterEvent& event )
{
   DBG_TRC( "type = %#zx, info = %s", static_cast< size_t >( event.data( )->type ), event.data( )->info.c_str( ) );
   switch( event.data( )->type )
   {
      case events::ePing::ping:
      {
         events::PingSlaveEvent::send_event( { events::ePing::ping, "Master -> Slave" }, base::eCommType::ETC );
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

void Master::process_event( const events::PingSlaveEvent& event )
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
         static size_t m_count = 0;
         if( 1 > ++m_count )
         {
            events::PingSlaveEvent::send_event( { events::ePing::ping, "Master -> Slave" }, base::eCommType::ETC );
         }
         else
         {
            m_count = 0;
            events::PingMasterEvent::send_event( { events::ePing::response, "Driver <- Master" }, base::eCommType::ITC );
         }
         break;
      }
      default:
      {
         break;
      }
   }
}



} // namespace application::master
