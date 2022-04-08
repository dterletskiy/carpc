#include "api/sys/application/Process.hpp"
#include "api/sys/comm/service/IClient.hpp"
#include "api/sys/comm/service/IProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IProxy"



namespace ev_i = carpc::events::service;
using namespace carpc::service;



IProxy::IProxy( const carpc::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_import )
   : IConnection( interface_type_id, role_name, is_import, IConnection::eType::Proxy )
{
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ServerConnected } );

   auto result = application::Process::instance( )->service_registry( ).register_client(
      signature( ), Address( context( ), id( ) )
   );
   // Send IPC notification information about registered client to ServiceBrocker
   // in case of there is no registered server with current service name and
   // client could export interface
   if( Registry::eResult::OK_NotPaired == result )
   {
      if( true == is_external( ) )
         ev_i::Action::Event::create_send( { ev_i::eAction::RegisterClient }, { signature( ), Address( context( ), id( ) ) } );
   }
}

IProxy::~IProxy( )
{
   auto result = application::Process::instance( )->service_registry( ).unregister_client(
      signature( ), Address( context( ), id( ) )
   );
   // Send IPC notification information about registered client to ServiceBrocker
   // in case of there client could exports interface
   if( Registry::eResult::OK_NotPaired == result )
   {
      ev_i::Action::Event::create_send( { ev_i::eAction::UnregisterClient }, { signature( ), Address( context( ), id( ) ) } );
   }
}

void IProxy::status( const Address& server_address, const eStatus connection_status )
{
   switch( connection_status )
   {
      case eStatus::Connected:
      {
         if( m_server.is_valid( ) )
         {
            if( server_address != m_server )
            {
               SYS_ERR( "Current connected server: %s. Newly connected server: %s", m_server.name( ).c_str( ), server_address.name( ).c_str( ) );
            }
            return;
         }

         ev_i::Status::Event::clear_notification( this, { signature( ), ev_i::eStatus::ServerConnected } );
         ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ServerDisconnected } );

         m_server = server_address;

         connected( m_server );

         for( auto item : m_client_set )
            item->connected( );

         break;
      }
      case eStatus::Disconnected:
      {
         ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ServerConnected } );
         ev_i::Status::Event::clear_notification( this, { signature( ), ev_i::eStatus::ServerDisconnected } );

         m_server = { };

         for( auto item : m_client_set )
            item->disconnected( );

         disconnected( server_address );

         break;
      }
   }
}

const bool IProxy::is_connected( ) const
{
   return m_server.is_valid( );
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
