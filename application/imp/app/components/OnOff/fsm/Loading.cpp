#include "imp/app/components/OnOff/fsm/Manager.hpp"
#include "imp/app/components/OnOff/fsm/Loading.hpp"



using namespace application::components::onoff;




Loading::Loading( Manager& manager )
   : base::fsm::TState< Types >( eID::Loading, "Loading" )
   , m_manager( manager )
{
}

const std::optional< Types::tID > Loading::enter( )
{
   return NoTransition;
}

void Loading::leave( )
{
}

const std::optional< Types::tID > Loading::process( const Data& data )
{
   return NoTransition;
}
