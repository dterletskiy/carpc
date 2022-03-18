#include "imp/app/components/OnOff/fsm/Manager.hpp"
#include "imp/app/components/OnOff/fsm/Loaded.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "LOADED"



using namespace application::components::onoff;




Loaded::Loaded( Manager& manager )
   : carpc::fsm::TState< Types >( eID::Loaded, "Loaded" )
   , m_manager( manager )
{
}

const std::optional< Types::tID > Loaded::enter( )
{
   return NoTransition;
}

void Loaded::leave( )
{
}

const std::optional< Types::tID > Loaded::process( const Data& data )
{
   MSG_VRB( );
   return NoTransition;
}
