#pragma once

#include "api/sys/services/onoff/Client.hpp"
#include "api/sys/tools/Performance.hpp"



namespace symbiont::interface::onoff {

   class Client
      : public carpc::onoff::Client
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

      private:
         void test( );
         carpc::tools::Performance m_performance = { "CARPC" };
   };

} // namespace symbiont::interface::onoff
