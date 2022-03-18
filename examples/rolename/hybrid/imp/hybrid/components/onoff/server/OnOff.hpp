#pragma once

#include "api/sys/services/onoff/Server.hpp"



namespace hybrid::interface::onoff {

   class Server
      : public carpc::onoff::Server
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

} // namespace hybrid::interface::onoff
