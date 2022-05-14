#include "api/service/onoff/2.0.0/Server.hpp"
#include "ServerImpl.hpp"

using namespace service::onoff::V2_0_0;

ServerImpl::ServerImpl( Server& server, const std::string& role_name )
   : IServer( role_name )
   , m_server( server )
{
}

void ServerImpl::connected( )
{
   m_server.connected( );
}

void ServerImpl::disconnected( )
{
   m_server.disconnected( );
}

void ServerImpl::request_trigger_state( const std::string& state, const std::size_t& timeout )
{
   m_server.request_trigger_state( state, timeout );
}

void ServerImpl::request_start( )
{
   m_server.request_start( );
}


