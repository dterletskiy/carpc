#include "api/sys/application/IThread.hpp"
#include "imp/sys/application/SystemEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SystemEventConsumer"


using namespace base::application;



SystemEventConsumer::SystemEventConsumer( IThread& app_thread )
   : m_app_thread( app_thread )
{
   events::system::System::Event::set_notification( this, { events::system::eID::boot } );
   events::system::System::Event::set_notification( this, { events::system::eID::shutdown } );
   events::system::System::Event::set_notification( this, { events::system::eID::ping } );
}

SystemEventConsumer::~SystemEventConsumer( )
{
   events::system::System::Event::clear_all_notifications( this );
}

void SystemEventConsumer::process_event( const events::system::System::Event& event )
{
   SYS_INF( "id = %s", events::system::c_str( event.info( ).id( ) ) );

   std::string message = "";
   if( event.data( ) ) message = event.data( )->message;

   switch( event.info( ).id( ) )
   {
      case events::system::eID::boot:
      {
         m_app_thread.boot( message );
         break;
      }
      case events::system::eID::shutdown:
      {
         m_app_thread.shutdown( message );
         break;
      }
      case events::system::eID::ping:
      {
         break;
      }
      default:
      {
         break;
      }
   }
}
