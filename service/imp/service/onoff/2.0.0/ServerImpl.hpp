#pragma once

#include "IServer.hpp"

namespace service::onoff::V2_0_0 {

   class Server;

   class ServerImpl
      : public IServer
   {
         friend class Server;

      public:
         ServerImpl( Server& server, const std::string& role_name );
         ~ServerImpl( ) override = default;

      private:
         void connected( ) override;
         void disconnected( ) override;

      public:
         void request_trigger_state( const std::string& _state, const std::size_t& _timeout ) override;
         void request_start( ) override;

      private:
         Server& m_server;
   };

} // namespace service::onoff::V2_0_0


