// Application
#include "imp/app/components/OnOff/Component.hpp"
#include "imp/app/components/Master/Component.hpp"
#include "imp/app/components/Slave/Component.hpp"
// Framework
#include "api/sys/application/main.hpp"



const carpc::application::Thread::Configuration::tVector services =
   {
        { "OnOff_Thread", { application::components::onoff::Component::creator }, 5 }
      , { "Master_Thread", { application::components::master::Component::creator }, 10 }
      , { "Slave_Thread", { application::components::slave::Component::creator }, 10 }
      // , { "Device_Service", { application::components::master::Component::creator, application::components::slave::Component::creator }, 10 }
   };



bool test( int argc, char** argv, char** envp )
{
   return true;
}
