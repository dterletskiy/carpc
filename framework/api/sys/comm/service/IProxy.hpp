#pragma once

#include "api/sys/comm/service/IConnection.hpp"



namespace carpc::service {

   class IClient;



   class IProxy : public IConnection
   {
      public:
         IProxy( const carpc::async::tAsyncTypeID&, const std::string&, const bool );
         ~IProxy( ) override;

      private:
         void status( const Address&, const eStatus ) override final;
         void connected( const Address& ) override = 0;
         void disconnected( const Address& ) override = 0;

      public:
         const Address& server( ) const;
         const bool is_connected( ) const override;
      protected:
         Address m_server = { };

      public:
         void register_client( IClient* );
         void unregister_client( IClient* );
      protected:
         std::set< IClient* > m_client_set;
   };



   inline
   const Address& IProxy::server( ) const
   {
      return m_server;
   }

} // namespace carpc::service
