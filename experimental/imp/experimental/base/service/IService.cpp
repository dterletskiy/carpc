#include "imp/experimental/base/application/Application.hpp"
#include "imp/experimental/base/service/Registry.hpp"
#include "imp/experimental/base/service/IService.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IService"



using namespace fw::service;



IService::IService( const tServiceID& id, const tServiceRole& role )
   : m_info( id, role )
   , mp_thread( application::Application::instance( ).current_thread( ) )
{
   DBG_MSG( "created" );
}

IService::~IService( )
{
   DBG_MSG( "destroyed" );
}
