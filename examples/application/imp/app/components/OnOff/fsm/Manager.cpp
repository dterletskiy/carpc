#include "imp/app/components/OnOff/fsm/Unloaded.hpp"
#include "imp/app/components/OnOff/fsm/Loading.hpp"
#include "imp/app/components/OnOff/fsm/Loaded.hpp"
#include "imp/app/components/OnOff/fsm/Manager.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff_FSM"



using namespace application::components::onoff;



struct Callback : public Manager::Subscriber
{
   void state_action( const Types::tID& state_uid, const eAction& action ) override
   {
      MSG_MARKER( );
   }
};



Manager::Manager( )
   : carpc::fsm::TStateManager< Types >( "OnOff" )
{
   register_state< Unloaded >( *this );
   register_state< Loading >( *this );
   register_state< Loaded >( *this );

   mp_subscriber = std::make_shared< Callback >( );
   subscribe( mp_subscriber );
}
