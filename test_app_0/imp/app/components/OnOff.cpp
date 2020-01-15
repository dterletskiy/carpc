// Application
#include "imp/app/components/OnOff.hpp"

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
   ServiceDSI::PingEvent::Event::set_notification( true, this );
}

OnOff::~OnOff( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   ServiceDSI::PingEvent::Event::set_notification( false, this );
}

namespace {
   const size_t s_events_count = 10;
   const std::vector< base::eCommType > s_comm_type_vector = { base::eCommType::ETC, base::eCommType::ITC, base::eCommType::IPC };
   auto s_comm_type_iterator = s_comm_type_vector.begin( );
}

bool OnOff::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   DBG_WRN( "Sending %ld %s events...", s_events_count, base::c_str( *s_comm_type_iterator ) );
   start_performance( );
   ServiceDSI::PingEvent::Event::create_send( { base::c_str( *s_comm_type_iterator ) }, *s_comm_type_iterator );
   return true;
}

void OnOff::process_event( const ServiceDSI::PingEvent::Event& event )
{
   // DBG_TRC( "info = %s", event.data( )->info.c_str( ) );

   static size_t count = 1;
   if( s_events_count > count )
   {
      ++count;
      ServiceDSI::PingEvent::Event::create_send( { base::c_str( *s_comm_type_iterator ) }, *s_comm_type_iterator );
   }
   else
   {
      stop_performance( );
      DBG_WRN( "Done %ld %s events...", s_events_count, base::c_str( *s_comm_type_iterator ) );
      if( s_comm_type_vector.end( ) != ++s_comm_type_iterator )
      {
         count = 1;
         DBG_WRN( "Sending %ld %s events...", s_events_count, base::c_str( *s_comm_type_iterator ) );
         start_performance( );
         ServiceDSI::PingEvent::Event::create_send( { base::c_str( *s_comm_type_iterator ) }, *s_comm_type_iterator );
      }
   }

   // DBG_TRC( "id = %#zx", static_cast< size_t >( event.id( ) ) );
   // switch( event.id( ) )
   // {
   //    case events::eEventID::request:
   //    {
   //       DBG_WRN( "request" );
   //       break;
   //    }
   //    case events::eEventID::response:
   //    {
   //       DBG_WRN( "response" );
   //       break;
   //    }
   //    default: break;
   // }
}



} // namespace application::onoff
