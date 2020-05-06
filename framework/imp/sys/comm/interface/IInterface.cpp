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
         case eInterface::Undefined:
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



IInterface::IInterface( const std::string& name, const std::string& role, const eType type, const eCommType comm_type )
   : m_type( type )
   , m_comm_type( comm_type )
   , m_name( name )
   , m_role( role )
{
   SYS_TRC( "created %s/%s", m_name.c_str( ), m_role.c_str( ) );
   mp_service_thread = ServiceProcess::instance( )->current_service( );
}

IInterface::~IInterface( )
{
   SYS_TRC( "destroyed %s/%s", m_name.c_str( ), m_role.c_str( ) );
}

void IInterface::process_event( const InterfaceEvent::Event& event )
{
   if( !(event.data( )) )
   {
      SYS_ERR( "event data is empty" );
      return;
   }

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
