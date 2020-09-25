#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/interface/IProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IProxy"



namespace ev_i = base::events::interface;
using namespace base::interface;



IProxy::IProxy( const base::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
   : IConnection( interface_type_id, role_name, is_import )
{
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ServerConnected } );

   auto result = ServiceProcess::instance( )->connection_db( ).register_client( signature( ), Address( this ) );
   // Send IPC notification information about registered client to ServiceBrocker
   // in case of there is no registered server with current service name and client could
   // export interface
   if( interface::ConnectionDB::eResult::OK_NotPaired == result )
   {
      if( true == is_external( ) )
         ev_i::Action::Event::create_send( { ev_i::eAction::RegisterClient }, { signature( ), this }, base::async::eCommType::IPC );
   }
}

IProxy::~IProxy( )
{
   auto result = ServiceProcess::instance( )->connection_db( ).unregister_client( signature( ), Address( this ) );
   // Send IPC notification information about registered client to ServiceBrocker
   // in case of there client could exports interface
   if( interface::ConnectionDB::eResult::OK_NotPaired == result )
   {
      ev_i::Action::Event::create_send( { ev_i::eAction::UnregisterClient }, { signature( ), this }, base::async::eCommType::IPC );
   }
}

void IProxy::connected( const Address& server )
{
   if( std::nullopt != m_server )
   {
      if( server != m_server )
      {
         SYS_ERR( "Current connected server: %p. Newly connected server: %p", m_server.value( ).ptr( ), server.ptr( ) );
      }
      return;
   }

   ev_i::Status::Event::clear_notification( this, { signature( ), ev_i::eStatus::ServerConnected } );
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ServerDisconnected } );

   m_server = server;

   for( auto item : m_client_set )
      item->connected( );

   connected( );
}

void IProxy::disconnected( const Address& server )
{
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ServerConnected } );
   ev_i::Status::Event::clear_notification( this, { signature( ), ev_i::eStatus::ServerDisconnected } );

   m_server.reset( );

   for( auto item : m_client_set )
      item->disconnected( );

   disconnected( );
}

const bool IProxy::is_connected( ) const
{
   return std::nullopt != m_server;
}

void IProxy::register_client( IClient* p_client )
{
   if( nullptr == p_client )
      return;

   m_client_set.emplace( p_client );
   if( is_connected( ) )
      p_client->connected( );
}

void IProxy::unregister_client( IClient* p_client )
{
   if( nullptr == p_client )
      return;

   m_client_set.erase( p_client );
}
