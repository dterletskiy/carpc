#pragma once

#include "carpc/comm/service/Address.hpp"
#include "carpc/comm/service/Signature.hpp"
#include "carpc/comm/service/Passport.hpp"
#include "carpc/comm/async/event/Event.hpp"



namespace carpc::events {

   namespace system {

      enum class eID : std::uint8_t { boot, shutdown, ping, undefined };
      const char* c_str( const eID );

      struct SystemData
      {
         std::string message;
      };
      DEFINE_EVENT( System, SystemData, carpc::async::id::TSignature< eID > );

   }



   namespace service {

      enum class eAction : std::uint8_t
      {
         RegisterServer,
         UnregisterServer,
         RegisterClient,
         UnregisterClient,
         Undefined
      };
      const char* c_str( const eAction );

      DEFINE_IPC_EVENT( Action, carpc::service::Passport, carpc::async::id::TSignature< eAction > );



      enum class eStatus : std::uint8_t
      {
         ServerConnected,
         ServerDisconnected,
         ClientConnected,
         ClientDisconnected,
         Undefined
      };
      const char* c_str( const eStatus );

      class SignatureStatus
      {
         public:
            SignatureStatus( ) = default;
            SignatureStatus( const carpc::service::Signature&, const eStatus );
            SignatureStatus( const SignatureStatus& );
            ~SignatureStatus( ) = default;

         public:
            const std::string dbg_name( ) const;
            bool operator<( const SignatureStatus& ) const;
            const bool to_stream( ipc::tStream& ) const;
            const bool from_stream( ipc::tStream& );

         public:
            const carpc::service::Signature& signature( ) const;
            const eStatus id( ) const;

         private:
            carpc::service::Signature      m_signature;
            eStatus                       m_id = eStatus::Undefined;
      };
      DEFINE_IPC_EVENT( Status, carpc::service::Address, SignatureStatus );

   }

} // namespace carpc::events
