#pragma once

#include "api/sys/oswrappers/linux/socket.hpp"
#include "api/sys/comm/interface/Types.hpp"
#include "api/sys/comm/event/Event.hpp"



namespace base::events {

   namespace service {

      enum class eID : size_t { boot, shutdown, ping, undefined };
      const char* c_str( const eID );

      struct Data
      {
         std::string message;
      };
      DEFINE_EVENT( Service, Data, TSignatureID< eID > );

   }

   namespace interface {

      enum class eAction : size_t
      {
         RegisterServer,
         UnregisterServer,
         RegisterClient,
         UnregisterClient,
         Undefined
      };
      const char* c_str( const eAction );

      struct ActionData
      {
         bool to_stream( base::dsi::tByteStream& ) const;
         bool from_stream( base::dsi::tByteStream& );

         base::interface::Signature    signature;
         const void*                   ptr = nullptr;
      };
      DEFINE_IPC_EVENT( Action, ActionData, TSignatureID< eAction > );



      enum class eStatus : size_t
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
            SignatureStatus( const base::interface::Signature&, const eStatus );
            SignatureStatus( const SignatureStatus& );
            ~SignatureStatus( ) = default;

         public:
            const std::string name( ) const;
            bool operator<( const SignatureStatus& ) const;
            const bool to_stream( dsi::tByteStream& ) const;
            const bool from_stream( dsi::tByteStream& );

         public:
            const base::interface::Signature& signature( ) const;
            const eStatus id( ) const;

         private:
            base::interface::Signature    m_signature;
            eStatus                       m_id = eStatus::Undefined;
      };


      DEFINE_IPC_EVENT( Status, base::interface::Address, SignatureStatus );

   }

} // namespace base::events
