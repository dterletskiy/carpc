#pragma once

#include "api/service/onoff/1.0.0/Server.hpp"



namespace symbiont::interface::onoff {

   class Server
      : public service::onoff::V1_0_0::Server
   {
      public:
         Server( );
         ~Server( );

      private:
         void connected( ) override;
         void disconnected( ) override;

      public:
         void request_start( ) override;
         void request_trigger_state( const std::string&, const size_t ) override;
   };

} // namespace symbiont::interface::onoff
