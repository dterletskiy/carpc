#pragma once

// Framework
#include "api/sys/application/RootComponent.hpp"
// Application
#include "imp/app/components/OnOff/client/OnOff_v.1.0.0.hpp"
#include "imp/app/components/OnOff/client/OnOff_v.2.0.0.hpp"



namespace application::components::onoff {

   class Component
      : public carpc::application::RootComponent
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_boot( const std::string& ) override;
         void process_shutdown( carpc::callback::tBlocker ) override;

      private:
         std::shared_ptr< client::V1_0_0::OnOff > mp_client_onoff_100;
         std::shared_ptr< client::V2_0_0::OnOff > mp_client_onoff_200;
   };

} // namespace application::components::onoff
