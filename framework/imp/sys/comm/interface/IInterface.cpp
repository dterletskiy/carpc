#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/IInterface.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IInterface"



namespace base {

   bool InterfaceEventData::to_buffer( base::ByteBufferT& buffer ) const
   {
      return buffer.push( ptr );
   }

   bool InterfaceEventData::from_buffer( base::ByteBufferT& buffer )
   {
      return buffer.pop( ptr );
   }

} // namespase base

INIT_EVENT( base::InterfaceEvent );



namespace base {



IInterface::IInterface( const std::string& name, const std::string& role )
   : m_name( name )
   , m_role( role )
{
   SYS_TRC( "created" );
   mp_service_thread = ServiceProcess::instance( )->current_service( );
}

IInterface::~IInterface( )
{
   SYS_TRC( "destroyed" );
}

void IInterface::process_event( const InterfaceEvent::Event& event )
{
   const void* const ptr = event.data( )->ptr;
   SYS_TRC( "connected side: %p", ptr );
   switch( event.info( ).id( ) )
   {
      case eInterface::ServerConnected:
      case eInterface::ClientConnected:
      {
         connected( ptr );
         break;
      }
      case eInterface::ServerDisconnected:
      case eInterface::ClientDisconnected:
      {
         disconnected( ptr );
         break;
      }
      default: break;
   }
}



} // namespace base
