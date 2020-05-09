#include "api/sys/comm/interface/IProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IProxy"



using namespace base;



IProxy::IProxy( const std::string& name, const std::string& role, const eCommType comm_type )
   : IInterface( name, role, comm_type )
{
   events::interface::Interface::Event::set_notification( this, { service_name( ), events::interface::eID::ServerConnected } );
   events::interface::Interface::Event::create_send( { service_name( ), events::interface::eID::ClientConnected }, { this }, m_comm_type );
}

IProxy::~IProxy( )
{
   events::interface::Interface::Event::clear_all_notifications( this );
   events::interface::Interface::Event::create_send( { service_name( ), events::interface::eID::ClientDisconnected }, { this }, m_comm_type );
}

void IProxy::connected( const void* const p_server )
{
   if( nullptr != mp_server )
   {
      if( p_server != mp_server )
      {
         SYS_ERR( "Current connected server: %p. Newly connected server: %p", mp_server, p_server );
      }
      return;
   }

   events::interface::Interface::Event::clear_notification( this, { service_name( ), events::interface::eID::ServerConnected } );
   events::interface::Interface::Event::set_notification( this, { service_name( ), events::interface::eID::ServerDisconnected } );
   events::interface::Interface::Event::create_send( { service_name( ), events::interface::eID::ClientConnected }, { this }, m_comm_type );

   mp_server = p_server;

   for( auto item : m_client_set )
      item->connected( );

   connected( );
}

void IProxy::disconnected( const void* const p_server )
{
   events::interface::Interface::Event::set_notification( this, { service_name( ), events::interface::eID::ServerConnected } );
   events::interface::Interface::Event::clear_notification( this, { service_name( ), events::interface::eID::ServerDisconnected } );

   mp_server = nullptr;

   for( auto item : m_client_set )
      item->disconnected( );

   disconnected( );
}

const bool IProxy::is_connected( ) const
{
   return nullptr != mp_server;
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
