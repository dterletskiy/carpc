#include "imp/experimental/base/component/Service.hpp"
#include "imp/experimental/base/application/Application.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Application"



using namespace fw::application;



std::unique_ptr< Application > Application::mp_instance = nullptr;



Application::Application( )
{
}

Application::~Application( )
{
}

Application& Application::instance( )
{
   if( nullptr == mp_instance )
      mp_instance.reset( new Application );

   return *( mp_instance.get( ) );
}

void Application::run( const std::list< Thread::Configuration >& configs )
{
   const std::list< Thread::Configuration > service_configs =
   {
      { "Service_Thread", { component::Service::creator } }
   };

   for( const auto config : service_configs )
   {
      auto p_thread = std::make_shared< Thread >( config.component_creators, config.name );
      if( p_thread )
         m_threads.push_back( p_thread );
   }

   for( const auto config : configs )
   {
      auto p_thread = std::make_shared< Thread >( config.component_creators, config.name );
      if( p_thread )
         m_threads.push_back( p_thread );
   }

   for( auto p_thread : m_threads )
      p_thread->run( );

   for( auto p_thread : m_threads )
      p_thread->wait( );
}

std::shared_ptr< Thread > Application::current_thread( ) const
{
   const auto this_id = std::this_thread::get_id( );
   for( auto p_thread : m_threads )
      if( this_id == p_thread->id( ) )
         return p_thread;

   return nullptr;
}