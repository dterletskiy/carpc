#include "imp/experimental/components/system/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Component_OnOff"



using namespace component::system;



std::unique_ptr< fw::component::Base > Component::creator( )
{
   return std::make_unique< Component >( );
}

Component::Component( )
   : fw::component::Base( "System" )
{
}

Component::~Component( )
{
}
