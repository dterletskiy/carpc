#include "imp/experimental/base/service/IStub.hpp"
#include "imp/experimental/base/service/IProxy.hpp"
#include "imp/experimental/base/service/Registry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IProxy"



using namespace fw::service;



IProxy::IProxy( const tServiceID& id, const tServiceRole& role )
   : IService( id, role )
{
}

IProxy::~IProxy( )
{
}

void IProxy::check_in( )
{
   Registry::instance( ).check_in( shared_from_this( ) );
}

void IProxy::check_out( )
{
   Registry::instance( ).check_out( shared_from_this( ) );
}

void IProxy::connected( std::shared_ptr< IStub > p_stub )
{
   if( auto sp_thread = thread( ) )
   {
      std::weak_ptr< IProxy > wp_proxy = shared_from_this( );
      auto function = [ wp_proxy, p_stub ]( )
      {
         if( auto sp_proxy = wp_proxy.lock( ) )
         {
            sp_proxy->mp_stub = p_stub;
            sp_proxy->connected( );
         }
      };
      sp_thread->post( function );
   }
   else
   {
      DBG_WRN( "current context is not exists" );
   }
}

void IProxy::disconnected( std::shared_ptr< IStub > p_stub )
{
   if( auto sp_thread = thread( ) )
   {
      std::weak_ptr< IProxy > wp_proxy = shared_from_this( );
      auto function = [ wp_proxy ]( )
      {
         if( auto sp_proxy = wp_proxy.lock( ) )
         {
            sp_proxy->mp_stub.reset( );
            sp_proxy->disconnected( );
         }
      };
      sp_thread->post( function );
   }
   else
   {
      DBG_WRN( "current context is not exists" );
   }
}

bool IProxy::is_connected( ) const
{
   return !mp_stub.expired( );
}

bool IProxy::is_disconnected( ) const
{
   return is_connected( );
}

void IProxy::request( application::Context::tFunction function )
{
   ++m_seq_id;
   DBG_INF( "seq_id: %zu", m_seq_id );

   std::weak_ptr< IProxy > wp_proxy = shared_from_this( );
   auto lambda = [ wp_proxy, seq_id = m_seq_id, function ]( )
   {
      if( auto sp_proxy = wp_proxy.lock( ) )
      {
         if( auto sp_stub = sp_proxy->mp_stub.lock( ) )
         {
            sp_stub->client_seq_id( seq_id, sp_proxy );
            function( );
         }
      }
   };

   if( auto sp_stub = mp_stub.lock( ) )
      sp_stub->thread( )->post( lambda );
}

