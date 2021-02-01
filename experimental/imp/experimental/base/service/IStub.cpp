#include "imp/experimental/base/service/IProxy.hpp"
#include "imp/experimental/base/service/IStub.hpp"
#include "imp/experimental/base/service/Registry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IStub"



using namespace fw::service;



IStub::IStub( const tServiceID& id, const tServiceRole& role )
   : IService( id, role )
{
}

IStub::~IStub( )
{
}

void IStub::check_in( )
{
   Registry::instance( ).check_in( shared_from_this( ) );
}

void IStub::check_out( )
{
   Registry::instance( ).check_out( shared_from_this( ) );
}

void IStub::connected( std::shared_ptr< IProxy > p_proxy )
{
   if( auto sp_thread = thread( ) )
   {
      std::weak_ptr< IStub > wp_stub = shared_from_this( );
      auto function = [ wp_stub, p_proxy ]( )
      {
         if( auto sp_stub = wp_stub.lock( ) )
         {
            sp_stub->m_proxies.insert( p_proxy );
            sp_stub->connected( );
         }
      };
      sp_thread->post( function );
   }
   else
   {
      MSG_WRN( "current context is not exists" );
   }
}

void IStub::disconnected( std::shared_ptr< IProxy > p_proxy )
{
   if( auto sp_thread = thread( ) )
   {
      std::weak_ptr< IStub > wp_stub = shared_from_this( );
      auto function = [ wp_stub, p_proxy ]( )
      {
         if( auto sp_stub = wp_stub.lock( ) )
         {
            sp_stub->m_proxies.erase( p_proxy );
            sp_stub->disconnected( );
         }
      };
      sp_thread->post( function );
   }
   else
   {
      MSG_WRN( "current context is not exists" );
   }
}

bool IStub::is_connected( ) const
{
   return !m_proxies.empty( );
}

bool IStub::is_disconnected( ) const
{
   return !is_connected( );
}
