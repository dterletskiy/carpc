#pragma once

#include "api/sys/services/onoff/Client.hpp"



namespace controller::components::onoff {

   class Client
      : public base::onoff::Client
   {
      public:
         Client( );
         ~Client( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      private:
         void response_trigger_state( const bool ) override;
         void request_trigger_state_failed( const base::service::eError& ) override;
         void on_current_state( const std::string& ) override;

      public:
         void request_start( );
         void request_trigger_state( const std::string&, const size_t );
   };

} // namespace controller::components::onoff
