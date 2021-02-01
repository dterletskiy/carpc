#include "imp/experimental/base/application/Application.hpp"
#include "imp/experimental/base/service/IStub.hpp"
#include "imp/experimental/base/service/IProxy.hpp"
#include "imp/experimental/base/service/Registry.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Registry"



using namespace fw::service;




std::unique_ptr< Registry > Registry::mp_instance = nullptr;



Registry::Registry( )
   : mp_thread ( application::Application::instance( ).current_thread( ) )
{
}

Registry::~Registry( )
{
}

Registry& Registry::instance( )
{
   if( nullptr == mp_instance )
      mp_instance.reset( new Registry );

   return *( mp_instance.get( ) );
}

void Registry::check_in( std::shared_ptr< IStub > p_stub )
{
   auto function = [ this, p_stub ]( ){ register_stub( p_stub ); };

   auto p_thread = mp_thread.lock( );
   if( p_thread )
      p_thread->post( function );
}

void Registry::check_out( std::shared_ptr< IStub > p_stub )
{
   auto function = [ this, p_stub ]( ){ unregister_stub( p_stub ); };

   auto p_thread = mp_thread.lock( );
   if( p_thread )
      p_thread->post( function );
}

void Registry::check_in( std::shared_ptr< IProxy > p_proxy )
{
   auto function = [ this, p_proxy ]( ){ register_proxy( p_proxy ); };

   auto p_thread = mp_thread.lock( );
   if( p_thread )
      p_thread->post( function );
}

void Registry::check_out( std::shared_ptr< IProxy > p_proxy )
{
   auto function = [ this, p_proxy ]( ){ unregister_proxy( p_proxy ); };

   auto p_thread = mp_thread.lock( );
   if( p_thread )
      p_thread->post( function );
}


void Registry::register_stub( std::shared_ptr< IStub > p_stub )
{
   MSG_DBG( "%s", p_stub->info( ).to_string( ).c_str( ) );

   auto result_pair = m_registry.insert( std::make_pair( p_stub->info( ), Connection{ } ) );
   auto iterator_registry = result_pair.first;
   bool result = result_pair.second;

   if( false == result && !iterator_registry->second.stub.expired( ) )
   {
      if( iterator_registry->second.stub.lock( ) == p_stub )
      {
         MSG_WRN( "current stub for current service already registered" );
      }
      else
      {
         MSG_WRN( "stub for current service already registered" );
      }
      return;
   }

   iterator_registry->second.stub = p_stub;

   for( auto wp_proxy : iterator_registry->second.proxies )
      if( auto sp_proxy = wp_proxy.lock( ) )
         sp_proxy->connected( p_stub );
}

void Registry::unregister_stub( std::shared_ptr< IStub > p_stub )
{
   MSG_DBG( "%s", p_stub->info( ).to_string( ).c_str( ) );

   auto iterator_registry = m_registry.find( p_stub->info( ) );
   if( m_registry.end( ) == iterator_registry )
   {
      MSG_WRN( "current service was not registered" );
      return;
   }

   if( iterator_registry->second.stub.expired( ) )
   {
      MSG_WRN( "stub was not registered for current service" );
      return;
   }

   iterator_registry->second.stub.reset( );

   for( auto wp_proxy : iterator_registry->second.proxies )
      if( auto sp_proxy = wp_proxy.lock( ) )
         sp_proxy->disconnected( p_stub );
}

void Registry::register_proxy( std::shared_ptr< IProxy > p_proxy )
{
   MSG_DBG( "%s", p_proxy->info( ).to_string( ).c_str( ) );

   auto result_pair = m_registry.insert( std::make_pair( p_proxy->info( ), Connection{ } ) );
   auto iterator_registry = result_pair.first;
   bool result = result_pair.second;

   iterator_registry->second.proxies.insert( p_proxy );

   if( auto sp_stub = iterator_registry->second.stub.lock( ) )
      sp_stub->connected( p_proxy );
}

void Registry::unregister_proxy( std::shared_ptr< IProxy > p_proxy )
{
   MSG_DBG( "%s", p_proxy->info( ).to_string( ).c_str( ) );

   auto iterator_registry = m_registry.find( p_proxy->info( ) );
   if( m_registry.end( ) == iterator_registry )
   {
      MSG_WRN( "current service was not registered" );
      return;
   }

   const std::size_t result = iterator_registry->second.proxies.erase( p_proxy );
   if( 0 == result )
   {
      MSG_WRN( "proxy was not registered for current service" );
      return;
   }

   if( auto sp_stub = iterator_registry->second.stub.lock( ) )
      sp_stub->disconnected( p_proxy );
}
