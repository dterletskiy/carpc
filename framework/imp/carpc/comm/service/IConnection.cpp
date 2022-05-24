#include "carpc/application/Process.hpp"
#include "carpc/comm/service/Address.hpp"
#include "carpc/comm/service/IConnection.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "IConnection"



namespace ev_i = carpc::events::service;
using namespace carpc::service;



IConnection::IConnection( const carpc::async::tAsyncTypeID& interface_type_id, const std::string& role, const bool is_external, const eType type )
   : m_signature( interface_type_id, role )
   , m_is_external( is_external )
   , m_type( type )
{
   if( false == m_context.is_valid( ) )
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
