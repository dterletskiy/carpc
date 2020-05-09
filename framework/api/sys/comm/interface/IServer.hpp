#pragma once

#include "api/sys/comm/interface/IInterface.hpp"



namespace base {

   class IServer : public IInterface
   {
      public:
         IServer( const std::string&, const std::string&, const eCommType );
         ~IServer( ) override;

      private:
         void connected( const void* const ) override final;
         void disconnected( const void* const ) override final;
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      protected:
         const bool is_connected( ) const override final;
      protected:
         std::set< const void* > mp_proxy_set;

   };

} // namespace base
