#pragma once

#include "api/sys/comm/interface/IConnection.hpp"



namespace base::interface {

   class IServer : public IConnection
   {
      public:
         IServer( const tAsyncTypeID&, const std::string&, const bool );
         ~IServer( ) override;

      private:
         void connected( const Address& ) override final;
         void disconnected( const Address& ) override final;
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      public:
         const bool is_connected( ) const override final;
      protected:
         Address::tSet m_proxy_set;

   };

} // namespace base::interface
