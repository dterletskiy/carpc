#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/IServer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IServer"



namespace ev_i = base::events::interface;
using namespace base::interface;



IServer::IServer( const base::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_export )
   : IConnection( interface_type_id, role_name, is_export )
{
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ClientConnected } );
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ClientDisconnected } );

   auto result = ServiceProcess::instance( )->connection_db( ).register_server( signature( ), Address( this ) );
   // Send IPC notification information about registered server to ServiceBrocker
   if( true == is_external( ) )
      ev_i::Action::Event::create_send( { ev_i::eAction::RegisterServer }, { signature( ), this }, base::async::eCommType::IPC );
}

IServer::~IServer( )
{
   auto result = ServiceProcess::instance( )->connection_db( ).unregister_server( signature( ), Address( this ) );
   // Send IPC notification information about registered server to ServiceBrocker
   if( true == is_external( ) )
      ev_i::Action::Event::create_send( { ev_i::eAction::UnregisterServer }, { signature( ), this }, base::async::eCommType::IPC );
}

void IServer::connected( const Address& proxy )
{
   // Check if current proxy was connected previously
   auto iterator = m_proxy_set.find( proxy );
   if( m_proxy_set.end( ) != iterator )
      return;

   m_proxy_set.emplace( proxy );
   connected( );
}

void IServer::disconnected( const Address& proxy )
{
   if( 0 != m_proxy_set.erase( proxy ) )
      disconnected( );
}

const bool IServer::is_connected( ) const
{
   return false == m_proxy_set.empty( );
}
