#pragma once

#include "IServer.hpp"



namespace service::onoff::V1_0_0 {

   class Server;

   class ServerImpl
      : public IServer
   {
         friend class Server;

      public:
         ServerImpl( Server&, const std::string& );
         ~ServerImpl( ) override = default;

      private:
         void connected( ) override;
         void disconnected( ) override;

      public:
         void request_start( ) override;
         void request_trigger_state( const std::string&, const size_t ) override;

      private:
         Server& m_server;
   };

} // namespace service::onoff::V1_0_0
