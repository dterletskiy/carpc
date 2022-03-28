#include "imp/app/components/OnOff/fsm/Manager.hpp"
#include "imp/app/components/OnOff/fsm/Loading.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "LOADING"



using namespace application::components::onoff;




Loading::Loading( Manager& manager )
   : carpc::fsm::TState< Types >( eID::Loading, "Loading" )
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

const std::optional< Types::tID > Loading::process( const Types::tData& data )
{
   MSG_VRB( );
   return NoTransition;
}
