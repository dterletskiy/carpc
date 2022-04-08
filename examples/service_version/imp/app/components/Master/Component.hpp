#pragma once

// Framework
#include "api/sys/application/Component.hpp"
// Application
#include "imp/app/components/Master/server/OnOff_v.1.0.0.hpp"



namespace application::components::master {

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
         server::V1_0_0::OnOff m_server_onoff;
   };

} // namespace application::components::master
