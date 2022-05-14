#include "ClientImpl.hpp"
#include "api/service/onoff/2.0.0/Client.hpp"

using namespace service::onoff::V2_0_0;

Client::Client( const std::string& role_name )
{
   mp_impl = new ClientImpl( *this, role_name );
}

Client::~Client( )
{
   delete mp_impl;
}

carpc::comm::sequence::ID Client::request_trigger_state( const std::string& state, const std::size_t& timeout )
{
   return mp_impl->request_trigger_state( state, timeout );
}

void Client::request_start( )
{
   return mp_impl->request_start( );
}

void Client::subscribe_current_state( )
{
   return mp_impl->subscribe_current_state( );
}

void Client::unsubscribe_current_state( )
{
   return mp_impl->unsubscribe_current_state( );
}


