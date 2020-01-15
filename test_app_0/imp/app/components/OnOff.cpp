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
   events::PingEvent::set_notification( true, this );
   DsiService::DsiPingEvent::set_notification( true, this );
}

OnOff::~OnOff( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::PingEvent::set_notification( false, this );
   DsiService::DsiPingEvent::set_notification( false, this );
}

namespace {
   const size_t events_count = 1;
}

bool OnOff::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   DBG_WRN( "Sending %ld ITC events...", events_count );
   start_performance( );
   // events::PingEvent::send_event( { events::ePing::ping, "OnOff -> Driver" }, base::eCommType::ITC );
   DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   return true;
}

void OnOff::process_event( const events::PingEvent& event )
{
   DBG_TRC( "type = %#zx, info = %s", static_cast< size_t >( event.data( )->type ), event.data( )->info.c_str( ) );

   events::PingEvent::send_event( { events::ePing::ping, "OnOff -> Driver" }, base::eCommType::ITC );


   // switch( event.data( )->type )
   // {
   //    case events::ePing::ping:
   //    {
   //       static size_t itc_events_count = 1;
   //       if( itc_events_count >= events_count )
   //       {
   //          stop_performance( );
   //          DBG_WRN( "Done %ld ITC events...", events_count );
   //          DBG_WRN( "Sending %ld IPC events...", events_count );
   //          DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //          // DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //       }
   //       else
   //       {
   //          ++itc_events_count;
   //          events::PingEvent::send_event( { events::ePing::ping, "OnOff -> Driver" }, base::eCommType::ITC );
   //       }
   //       break;
   //    }
   //    default: break;
   // }
}

void OnOff::process_event( const DsiService::DsiPingEvent& event )
{
   DBG_TRC( "type = %#zx, info = %s", static_cast< size_t >( event.data( )->type ), event.data( )->info.c_str( ) );

   DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );

   // switch( event.data( )->type )
   // {
   //    case events::ePing::ping:
   //    {
   //       static size_t ipc_events_count = 1;
   //       if( ipc_events_count >= events_count )
   //       {
   //          stop_performance( );
   //          DBG_WRN( "Done %ld IPC events...", events_count );
   //          exit( );
   //       }
   //       else
   //       {
   //          ++ipc_events_count;
   //          DsiService::DsiPingEvent::send_event( { events::ePing::ping, "OnOff -> Driver (via DSI)" } );
   //       }
   //       break;
   //    }
   //    default: break;
   // }
}


} // namespace application::onoff
