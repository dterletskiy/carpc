// Application
#include "imp/hmi/components/onoff/Component.hpp"
// Framework
#include "api/sys/application/main.hpp"



const base::application::Thread::Configuration::tVector services =
   {
        { "OnOff_Service", { hmi::components::onoff::Component::creator }, 5 }
   };
const char* const application_name{ "HMI" };



bool test( int argc, char** argv, char** envp )
{
   return true;
}

