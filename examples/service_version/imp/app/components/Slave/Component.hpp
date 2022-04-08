#pragma once

// Framework
#include "api/sys/application/Component.hpp"
// Application
#include "imp/app/components/Slave/server/OnOff_v.2.0.0.hpp"



namespace application::components::slave {

   class Component
      : public carpc::application::Component
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         server::V2_0_0::OnOff m_onoff_server;
   };

} // namespace application::components::slave
