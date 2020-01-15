#include "Root.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "Root"


namespace base {



Root::Root( )
{
   SYS_INF( "%p", this );
   SysEvent::set_notification( true, this );
}

Root::~Root( )
{
   SysEvent::set_notification( false, this );
}

void Root::exit( ) const
{
   ServiceEvent::send_event( { eServiceCommand::shutdown, "shutdown" }, eCommType::ITC );
}

void Root::process_event( const SysEvent& event )
{
   SYS_INF( "command = %#x, info = %s", event.data( )->command, event.data( )->info.c_str( ) );
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
