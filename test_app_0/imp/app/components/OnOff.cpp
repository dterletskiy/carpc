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
   events::PingEventETC::Event::set_notification( true, this );
   events::PingEventITC::Event::set_notification( true, this );
   ServiceDSI::PingEventDSI::Event::set_notification( true, this );
   events::EventEx::Event::set_notification( true, this );
}

OnOff::~OnOff( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::PingEventETC::Event::set_notification( false, this );
   events::PingEventITC::Event::set_notification( false, this );
   ServiceDSI::PingEventDSI::Event::set_notification( false, this );
   events::EventEx::Event::set_notification( false, this );
}

namespace {
   const size_t events_count = 1;
}

bool OnOff::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   DBG_WRN( "Sending %ld ETC events...", events_count );
   start_performance( );
   events::PingEventETC::Event::create_send( { "OnOff event ETC" }, base::eCommType::ETC );
   return true;
}

void OnOff::process_event( const events::PingEventETC::Event& event )
{
   DBG_TRC( "info = %s", event.data( )->info.c_str( ) );

   static size_t itc_events_count = 1;
   if( itc_events_count >= events_count )
   {
      stop_performance( );
      DBG_WRN( "Done %ld ETC events...", events_count );
      DBG_WRN( "Sending %ld ITC events...", events_count );
      start_performance( );
      events::PingEventITC::Event::create_send( { "OnOff event ITC" }, base::eCommType::ITC );
   }
   else
   {
      ++itc_events_count;
      events::PingEventETC::Event::create_send( { "OnOff" }, base::eCommType::ETC );
   }
}

void OnOff::process_event( const events::PingEventITC::Event& event )
{
   DBG_TRC( "info = %s", event.data( )->info.c_str( ) );

   static size_t itc_events_count = 1;
   if( itc_events_count >= events_count )
   {
      stop_performance( );
      DBG_WRN( "Done %ld ITC events...", events_count );
      DBG_WRN( "Sending %ld DSI events...", events_count );
      start_performance( );
      ServiceDSI::PingEventDSI::Event::create_send( { "OnOff event DSI" } );
   }
   else
   {
      ++itc_events_count;
      events::PingEventITC::Event::create_send( { "OnOff event ITC" }, base::eCommType::ITC );
   }
}

void OnOff::process_event( const ServiceDSI::PingEventDSI::Event& event )
{
   DBG_TRC( "info = %s", event.data( )->info.c_str( ) );

   static size_t ipc_events_count = 1;
   if( ipc_events_count >= events_count )
   {
      stop_performance( );
      DBG_WRN( "Done %ld DSI events...", events_count );
      auto event = events::EventEx::Event::create( { "OnOff extended event ETC" }, base::eCommType::ETC );
      event->id( events::eEventID::request );
      event->send( );
   }
   else
   {
      ++ipc_events_count;
      ServiceDSI::PingEventDSI::Event::create_send( { "OnOff event DSI" } );
   }

   switch( event.id( ) )
   {
      case events::eEventID::request: DBG_WRN( "request" ); break;
      case events::eEventID::response: DBG_WRN( "response" ); break;
      default: break;
   }
}

void OnOff::process_event( const events::EventEx::Event& event )
{
   DBG_TRC( "info = %s", event.data( )->info.c_str( ) );

   DBG_TRC( "id = %#zx", static_cast< size_t >( event.id( ) ) );
   switch( event.id( ) )
   {
      case events::eEventID::request:
      {
         DBG_WRN( "request" );
         auto event = events::EventEx::Event::create( { "OnOff extended event ETC" }, base::eCommType::ETC );
         event->id( events::eEventID::response );
         event->send( );
         break;
      }
      case events::eEventID::response:
      {
         DBG_WRN( "response" );
         shutdown( );
         break;
      }
      default: break;
   }
}



} // namespace application::onoff
