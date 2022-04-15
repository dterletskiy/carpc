#pragma once

#include "IClient.hpp"



namespace service::onoff::V1_0_0 {

   class Client;

   class ClientImpl
      : public IClient
   {
         friend class Client;

      public:
         ClientImpl( Client&, const std::string& );
         ~ClientImpl( ) override = default;

      private:
         void connected( ) override;
         void disconnected( ) override;

      private:
         void response_trigger_state( const bool ) override;
         void request_trigger_state_failed( const carpc::service::eError& ) override;
         void on_current_state( const std::string& ) override;

      private:
         Client& m_client;
   };

} // namespace service::onoff::V1_0_0
