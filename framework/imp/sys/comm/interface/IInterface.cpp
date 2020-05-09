#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/IInterface.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IInterface"



using namespace base;



IInterface::IInterface( const std::string& name, const std::string& role, const eCommType comm_type )
   : m_comm_type( comm_type )
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

void IInterface::process_event( const events::interface::Interface::Event& event )
{
   if( !(event.data( )) )
   {
      SYS_ERR( "event data is empty" );
      return;
   }

   const void* const ptr = event.data( )->ptr;
   switch( event.info( ).id( ) )
   {
      case events::interface::eID::ServerConnected:
      case events::interface::eID::ClientConnected:
      {
         SYS_TRC( "connected side: %p", ptr );
         connected( ptr );
         break;
      }
      case events::interface::eID::ServerDisconnected:
      case events::interface::eID::ClientDisconnected:
      {
         SYS_TRC( "disconnected side: %p", ptr );
         disconnected( ptr );
         break;
      }
      default: break;
   }
}
