#pragma once

#include "IClient.hpp"

namespace service::onoff::V2_0_0 {

   class Client;

   class ClientImpl
      : public IClient
   {
         friend class Client;

      public:
         ClientImpl( Client& client, const std::string& role_name );
         ~ClientImpl( ) override = default;

      private:
         void connected( ) override;
         void disconnected( ) override;

      public:
         void response_trigger_state( const bool& _result ) override;
         virtual void request_trigger_state_failed( const carpc::service::eError& error ) override;

      public:
         void on_current_state( const std::string& _value ) override;

      private:
         Client& m_client;
   };

} // namespace service::onoff::V2_0_0


