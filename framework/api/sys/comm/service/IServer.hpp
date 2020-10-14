#pragma once

#include "api/sys/comm/service/IConnection.hpp"



namespace base::service {

   class IServer : public IConnection
   {
      public:
         IServer( const base::async::tAsyncTypeID&, const std::string&, const bool );
         ~IServer( ) override;

      private:
         void status( const Address&, const eStatus ) override final;
         void connected( const Address& ) override = 0;
         void disconnected( const Address& ) override = 0;

      public:
         const bool is_connected( ) const override final;
      protected:
         Address::tSet m_proxy_set;

   };

} // namespace base::service
