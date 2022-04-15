#pragma once

#include "api/service/onoff/1.0.0/Client.hpp"



namespace hybrid::interface::onoff {

   class Client
      : public service::onoff::V1_0_0::Client
   {
      public:
         Client( );
         ~Client( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      private:
         void response_trigger_state( const bool ) override;
         void request_trigger_state_failed( const carpc::service::eError& ) override;
         void on_current_state( const std::string& ) override;

      public:
         void request_start( );
         void request_trigger_state( const std::string&, const size_t );
   };

} // namespace hybrid::interface::onoff
