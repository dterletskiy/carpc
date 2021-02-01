#include "api/sys/application/Process.hpp"
#include "api/sys/comm/service/Address.hpp"
#include "api/sys/comm/service/IConnection.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IConnection"



namespace ev_i = base::events::service;
using namespace base::service;



IConnection::IConnection( const base::async::tAsyncTypeID& interface_type_id, const std::string& role, const bool is_external )
   : m_signature( interface_type_id, role )
   , m_context( application::Context::eInitType::Auto )
   , m_is_external( is_external )
{
   if( application::Process::instance( )->current_thread( )->id( ).is_invalid( ) )
   {
      SYS_ERR( "creating connection '%s' not from application thread", m_signature.name( ).c_str( ) );
      return;
   }

   SYS_VRB( "created %s", m_signature.name( ).c_str( ) );
}

IConnection::~IConnection( )
{
   SYS_VRB( "destroyed %s", m_signature.name( ).c_str( ) );
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
         SYS_VRB( "connected side: %s", address.name( ).c_str( ) );
         status( address, eStatus::Connected );
         break;
      }
      case ev_i::eStatus::ServerDisconnected:
      case ev_i::eStatus::ClientDisconnected:
      {
         SYS_VRB( "disconnected side: %s", address.name( ).c_str( ) );
         status( address, eStatus::Disconnected );
         break;
      }
      default: break;
   }
}
