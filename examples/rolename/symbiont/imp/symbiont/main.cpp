// Application
#include "imp/symbiont/components/onoff/Component.hpp"
// Framework
#include "api/sys/application/main.hpp"



const carpc::application::Thread::Configuration::tVector services =
   {
        { "OnOff_Service", { symbiont::components::onoff::Component::creator }, 5 }
   };



bool test( int argc, char** argv, char** envp )
{
   return true;
}

