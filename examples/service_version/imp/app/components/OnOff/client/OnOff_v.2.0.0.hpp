#pragma once

#include <functional>
#include "api/app/services/onoff/2.0/Client.hpp"



namespace application::components::onoff::client::V2_0_0 {

   class OnOff
      : public service::onoff::V2_0_0::Client
   {
      public:
         OnOff( const std::string& );
         ~OnOff( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      private:
         void response_trigger_state( const bool ) override;
         void request_trigger_state_failed( const carpc::service::eError& ) override;
         void on_current_state( const std::string& ) override;

      public:
         void request_trigger_state( const std::string&, const size_t );
   };

} // namespace application::components::onoff::client::V2_0_0
