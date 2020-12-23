#include "imp/app/components/OnOff/fsm/Manager.hpp"
#include "imp/app/components/OnOff/fsm/Unloaded.hpp"



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
   return NoTransition;
}
