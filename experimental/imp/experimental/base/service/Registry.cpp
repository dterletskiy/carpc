#include "imp/experimental/base/application/Application.hpp"
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

void Registry::register_stub( std::shared_ptr< IStub > p_stub )
{
}

void Registry::unregister_stub( std::shared_ptr< IStub > p_stub )
{
}

void Registry::register_proxy( std::shared_ptr< IProxy > p_proxy )
{
}

void Registry::unregister_proxy( std::shared_ptr< IProxy > p_proxy )
{
}
