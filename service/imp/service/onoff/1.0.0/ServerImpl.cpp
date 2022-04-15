#include "api/service/onoff/1.0.0/Server.hpp"
#include "ServerImpl.hpp"



using namespace service::onoff::V1_0_0;



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

void ServerImpl::request_start( )
{
   m_server.request_start( );
}

void ServerImpl::request_trigger_state( const std::string& state, const size_t delay )
{
   m_server.request_trigger_state( state, delay );
}
