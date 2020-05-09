#pragma once

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

      enum class eID : size_t { ServerConnected, ServerDisconnected, ClientConnected, ClientDisconnected, Undefined };
      const char* c_str( const eID );

      struct Data
      {
         bool to_buffer( base::ByteBufferT& ) const;
         bool from_buffer( base::ByteBufferT& );

         const void* ptr = nullptr;
      };
      DEFINE_IPC_EVENT( Interface, Data, TSignatureRR< eID > );

   }

} // namespace base::events
