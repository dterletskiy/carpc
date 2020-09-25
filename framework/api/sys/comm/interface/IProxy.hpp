#pragma once

#include "api/sys/comm/interface/IConnection.hpp"
#include "api/sys/comm/interface/IClient.hpp"



namespace base::interface {

   class IProxy : public IConnection
   {
      public:
         IProxy( const base::async::tAsyncTypeID&, const std::string&, const bool );
         ~IProxy( ) override;

      private:
         void connected( const Address& ) override final;
         void disconnected( const Address& ) override final;
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      public:
         const Address::tOpt& server( ) const;
         const bool is_connected( ) const override;
      protected:
         Address::tOpt m_server = std::nullopt;

      public:
         void register_client( IClient* );
         void unregister_client( IClient* );
      protected:
         std::set< IClient* > m_client_set;
   };

   inline
   const Address::tOpt& IProxy::server( ) const
   {
      return m_server;
   }

} // namespace base::interface
