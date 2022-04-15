#include "ServerImpl.hpp"
#include "api/service/onoff/1.0.0/Server.hpp"



using namespace service::onoff::V1_0_0;



Server::Server( const std::string& role_name )
{
   mp_impl = new ServerImpl( *this, role_name );
}

Server::~Server( )
{
   delete mp_impl;
}

void Server::response_trigger_state( const bool result )
{
   mp_impl->response_trigger_state( result );
}

void Server::current_state( const std::string& state )
{
   mp_impl->current_state( state );
}

const std::string& Server::current_state( ) const
{
   return mp_impl->current_state( );
}

const carpc::comm::sequence::ID Server::unblock_request( )
{
   return mp_impl->unblock_request( );
}

void Server::prepare_response( const carpc::comm::sequence::ID& seq_id )
{
   mp_impl->prepare_response( seq_id );
}
