#include "imp/experimental/base/service/IStub.hpp"
#include "imp/experimental/base/service/IProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IProxy"



using namespace fw::service;



IProxy::IProxy( )
{
   // Registry::register_stub( std::shared_from_this( ) )
}

IProxy::~IProxy( )
{
}

void IProxy::connected( std::shared_ptr< IStub > p_stub )
{
   mp_stub = p_stub;
}

void IProxy::disconnected( std::shared_ptr< IStub > p_stub )
{
   mp_stub.reset( );
}

bool IProxy::is_connected( ) const
{
   return !mp_stub.expired( );
}

bool IProxy::is_disconnected( ) const
{
   return is_connected( );
}
