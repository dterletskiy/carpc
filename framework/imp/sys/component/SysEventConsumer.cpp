#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/component/RootComponent.hpp"
#include "imp/sys/component/SysEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SysEventConsumer"


namespace base {



SysEventConsumer* SysEventConsumer::mp_instance;

SysEventConsumer::SysEventConsumer( RootComponent* p_root_component )
   : mp_root_component( p_root_component )
{
   SysEvent::Event::set_notification( true, this );
}

SysEventConsumer::~SysEventConsumer( )
{
   SysEvent::Event::set_notification( false, this );
}

SysEventConsumer* SysEventConsumer::instance( RootComponent* p_root_component )
{
   os::Mutex mutex( true );
   if( !mp_instance )
   {
      mp_instance = new SysEventConsumer( p_root_component );
   }
   else
   {
      if( p_root_component == mp_instance->mp_root_component )
      {
         SYS_ERR( "there is not possible to have more then one root component" );
      }
   }

   return mp_instance;
}

void SysEventConsumer::process_event( const SysEvent::Event& event )
{
   SYS_INF( "command = %#zx, info = %s", static_cast< size_t >( event.data( )->command ), event.data( )->info.c_str( ) );
   switch( event.data( )->command )
   {
      case eSysCommand::boot:
      {
         mp_root_component->boot( event.data( )->info );
         break;
      }
      case eSysCommand::shutdown:
      {
         // mp_root_component->shutdown( event.data( )->info );
         break;
      }
      case eSysCommand::ping:
      {
         break;
      }
      default:
      {
         break;
      }
   }
}



} // namespace base
