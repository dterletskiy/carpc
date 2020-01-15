#include "api/sys/events/ServiceEvent.hpp"
#include "api/sys/component/RootComponent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "RootComponent"


namespace base {



RootComponent::RootComponent( const ServicePtr p_service, const std::string& name )
   : Component( p_service, name )
{
   SYS_INF( "%p", this );
   SysEvent::set_notification( true, this );
}

RootComponent::~RootComponent( )
{
   SysEvent::set_notification( false, this );
}

void RootComponent::exit( ) const
{
   ServiceEvent::send_event( { eServiceCommand::shutdown, "shutdown" }, eCommType::ITC );
}

void RootComponent::process_event( const SysEvent& event )
{
   SYS_INF( "command = %#zx, info = %s", static_cast< size_t >( event.data( )->command ), event.data( )->info.c_str( ) );
   switch( event.data( )->command )
   {
      case eSysCommand::boot:
      {
         boot( event.data( )->info );
         break;
      }
      case eSysCommand::shutdown:
      {
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
