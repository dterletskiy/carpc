// Application
#include "imp/core/components/onoff/Component.hpp"
// Framework
#include "api/sys/application/main.hpp"



const base::application::Thread::Configuration::tVector services =
   {
        { "OnOff_Service", { core::components::onoff::Component::creator }, 5 }
   };



bool test( int argc, char** argv, char** envp )
{
   return true;
}

