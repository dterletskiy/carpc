#include "imp/experimental/base/application/Application.hpp"
#include "imp/experimental/base/application/Thread.hpp"
#include "imp/experimental/base/service/Registry.hpp"
#include "imp/experimental/base/service/IService.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IService"



using namespace fw::service;



IService::IService( )
   : mp_thread ( application::Application::instance( ).current_thread( ) )
{
}

IService::~IService( )
{
}

const tServiceID& IService::service_id( ) const
{
   return m_service_id;
}
