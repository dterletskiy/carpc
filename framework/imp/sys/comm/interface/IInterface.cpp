#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/IInterface.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IInterface"



namespace base {

   const char* c_str( const eInterface id )
   {
      switch( id )
      {
         case eInterface::ServerConnected:      return "eInterface::ServerConnected";
         case eInterface::ServerDisconnected:   return "eInterface::ServerDisconnected";
         case eInterface::ClientConnected:      return "eInterface::ClientConnected";
         case eInterface::ClientDisconnected:   return "eInterface::ClientDisconnected";
         case eInterface::Undefined:            return "eInterface::Undefined";
         default:                               return "eInterface::Undefined";
      }
   }

   bool InterfaceEventData::to_buffer( base::ByteBufferT& buffer ) const
   {
      return buffer.push( ptr );
   }

   bool InterfaceEventData::from_buffer( base::ByteBufferT& buffer )
   {
      return buffer.pop( ptr );
   }

} // namespase base



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
