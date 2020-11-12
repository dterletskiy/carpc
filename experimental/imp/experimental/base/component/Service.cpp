#include "imp/experimental/base/service/Registry.hpp"
#include "imp/experimental/base/component/Service.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Service_OnOff"



using namespace fw::component;



std::unique_ptr< Base > Service::creator( )
{
   return std::make_unique< Service >( );
}

Service::Service( )
   : Base( "Service" )
{
   fw::service::Registry::instance( );
}

Service::~Service( )
{
}
