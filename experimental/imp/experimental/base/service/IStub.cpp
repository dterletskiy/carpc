#include "imp/experimental/base/service/IProxy.hpp"
#include "imp/experimental/base/service/IStub.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IStub"



using namespace fw::service;



IStub::IStub( )
{
   // Registry::register_stub( std::shared_from_this( ) )
}

IStub::~IStub( )
{
}

void IStub::connected( std::shared_ptr< IProxy > p_proxy )
{
   auto pair_result = m_proxies.insert( p_proxy );
}

void IStub::disconnected( std::shared_ptr< IProxy > p_proxy )
{
   auto result = m_proxies.erase( p_proxy );
}

bool IStub::is_connected( ) const
{
   return !m_proxies.empty( );
}

bool IStub::is_disconnected( ) const
{
   return !is_connected( );
}
