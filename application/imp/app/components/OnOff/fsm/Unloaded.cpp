#include "imp/app/components/OnOff/fsm/Manager.hpp"
#include "imp/app/components/OnOff/fsm/Unloaded.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "UNLOADED"



using namespace application::components::onoff;




Unloaded::Unloaded( Manager& manager )
   : base::fsm::TState< Types >( eID::Unloaded, "Unloaded" )
   , m_manager( manager )
{
}

const std::optional< Types::tID > Unloaded::enter( )
{
   return eID::Loading;
}

void Unloaded::leave( )
{
}

const std::optional< Types::tID > Unloaded::process( const Data& data )
{
   MSG_VRB( );
   return NoTransition;
}
