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

void Client::request_start( )
{
   mp_impl->request_start( );
}


void Client::request_trigger_state( const std::string& state, const size_t delay )
{
   mp_impl->request_trigger_state( state, delay );
}

void Client::subscribe_current_state( )
{
   mp_impl->subscribe_current_state( );
}
void Client::unsubscribe_current_state( )
{
   mp_impl->unsubscribe_current_state( );
}
