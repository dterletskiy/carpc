#include "api/service/onoff/2.0.0/Client.hpp"
#include "ClientImpl.hpp"



using namespace service::onoff::V2_0_0;



ClientImpl::ClientImpl( Client& client, const std::string& role_name )
   : IClient( role_name )
   , m_client( client )
{
}

void ClientImpl::connected( )
{
   m_client.connected( );
}

void ClientImpl::disconnected( )
{
   m_client.disconnected( );
}

void ClientImpl::response_trigger_state( const bool result )
{
   m_client.response_trigger_state( result );
}

void ClientImpl::request_trigger_state_failed( const carpc::service::eError& error )
{
   m_client.request_trigger_state_failed( error );
}

void ClientImpl::on_current_state( const std::string& value )
{
   m_client.on_current_state( value );
}
