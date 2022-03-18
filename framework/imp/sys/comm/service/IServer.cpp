#include "api/sys/application/Process.hpp"
#include "api/sys/comm/service/IServer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IServer"



namespace ev_i = carpc::events::service;
using namespace carpc::service;



IServer::IServer( const carpc::async::tAsyncTypeID& interface_type_id, const std::string& role_name, const bool is_export )
   : IConnection( interface_type_id, role_name, is_export )
{
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ClientConnected } );
   ev_i::Status::Event::set_notification( this, { signature( ), ev_i::eStatus::ClientDisconnected } );

   auto result = application::Process::instance( )->service_registry( ).register_server(
      signature( ), Address( context( ), id( ) )
   );
   // Send IPC notification information about registered server to ServiceBrocker
   if( true == is_external( ) )
      ev_i::Action::Event::create_send( { ev_i::eAction::RegisterServer }, { signature( ), Address( context( ), id( ) ) } );  
}

IServer::~IServer( )
{
   auto result = application::Process::instance( )->service_registry( ).unregister_server(
      signature( ), Address( context( ), id( ) )
   );
   // Send IPC notification information about registered server to ServiceBrocker
   if( true == is_external( ) )
      ev_i::Action::Event::create_send( { ev_i::eAction::UnregisterServer }, { signature( ), Address( context( ), id( ) ) } );
}

void IServer::status( const Address& proxy_address, const eStatus connection_status )
{
   switch( connection_status )
   {
      case eStatus::Connected:
      {
         // Check if current proxy was connected previously
         auto iterator = m_proxy_set.find( proxy_address );
         if( m_proxy_set.end( ) != iterator )
            return;

         m_proxy_set.emplace( proxy_address );
         connected( proxy_address );

         break;
      }
      case eStatus::Disconnected:
      {
         if( 0 != m_proxy_set.erase( proxy_address ) )
            disconnected( proxy_address );

         break;
      }
   }
}

const bool IServer::is_connected( ) const
{
   return false == m_proxy_set.empty( );
}
