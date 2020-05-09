#include "api/sys/comm/interface/IServer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IServer"



using namespace base;



IServer::IServer( const std::string& name, const std::string& role, const eCommType comm_type )
   : IInterface( name, role, comm_type )
{
   events::interface::Interface::Event::set_notification( this, { service_name( ), events::interface::eID::ClientConnected } );
   events::interface::Interface::Event::set_notification( this, { service_name( ), events::interface::eID::ClientDisconnected } );
   events::interface::Interface::Event::create_send( { service_name( ), events::interface::eID::ServerConnected }, { this }, m_comm_type );
}

IServer::~IServer( )
{
   events::interface::Interface::Event::clear_all_notifications( this );
   events::interface::Interface::Event::create_send( { service_name( ), events::interface::eID::ServerDisconnected }, { this }, m_comm_type );
}

void IServer::connected( const void* const p_proxy )
{
   // Check if current proxy was connected previously
   auto iterator = mp_proxy_set.find( p_proxy );
   if( mp_proxy_set.end( ) != iterator )
      return;

   // Send event to all consumers (just connected proxy) that server is connected
   events::interface::Interface::Event::create_send( { service_name( ), events::interface::eID::ServerConnected }, { this }, m_comm_type );

   mp_proxy_set.emplace( p_proxy );
   connected( );
}

void IServer::disconnected( const void* const p_proxy )
{
   mp_proxy_set.erase( p_proxy );
   disconnected( );
}

const bool IServer::is_connected( ) const
{
   return false == mp_proxy_set.empty( );
}
