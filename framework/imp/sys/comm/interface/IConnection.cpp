#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/IConnection.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IConnection"



namespace ev_i = base::events::interface;
using namespace base::interface;



IConnection::IConnection( const tAsyncTypeID& interface_type_id, const std::string& role, const bool is_external )
   : m_is_external( is_external )
   , m_signature( interface_type_id, role )
{
   SYS_TRC( "created %s", m_signature.name( ).c_str( ) );
   mp_service_thread = ServiceProcess::instance( )->current_service( );
}

IConnection::~IConnection( )
{
   SYS_TRC( "destroyed %s", m_signature.name( ).c_str( ) );
   ev_i::Status::Event::clear_all_notifications( this );
}

void IConnection::process_event( const ev_i::Status::Event& event )
{
   if( !(event.data( )) )
   {
      SYS_ERR( "event data is empty" );
      return;
   }

   const Address& address = *( event.data( ) );
   switch( event.info( ).id( ) )
   {
      case ev_i::eStatus::ServerConnected:
      case ev_i::eStatus::ClientConnected:
      {
         SYS_TRC( "connected side: %p", address.ptr( ) );
         connected( address );
         break;
      }
      case ev_i::eStatus::ServerDisconnected:
      case ev_i::eStatus::ClientDisconnected:
      {
         SYS_TRC( "disconnected side: %p", address.ptr( ) );
         disconnected( address );
         break;
      }
      default: break;
   }
}
