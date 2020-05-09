#pragma once

#include "api/sys/comm/interface/IInterface.hpp"
#include "api/sys/comm/interface/IClient.hpp"



namespace base {

   class IProxy : public IInterface
   {
      public:
         IProxy( const std::string&, const std::string&, const eCommType );
         ~IProxy( ) override;

      private:
         void connected( const void* const ) override final;
         void disconnected( const void* const ) override final;
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      protected:
         const bool is_connected( ) const override;
      protected:
         const void* mp_server = nullptr;

      public:
         void register_client( IClient* );
         void unregister_client( IClient* );
      protected:
         std::set< IClient* > m_client_set;

   };

} // namespace base
