#pragma once

#include "api/sys/comm/interface/Types.hpp"



namespace base::interface {

   class ConnectionDB
   {
      struct Connection
      {
         Address::tOpt server = std::nullopt;
         Address::tSet clients;
      };

      public:
         using tSptr = std::shared_ptr< ConnectionDB >;

      public:
         enum class eResult { OK_NotPaired, OK_Paired, Duplicate, NotFound, Error };

      public:
         ConnectionDB( ) = default;
         ~ConnectionDB( ) = default;
      private:
         ConnectionDB( const ConnectionDB& ) = delete;
         ConnectionDB& operator=( const ConnectionDB& ) = delete;

      public:
         eResult register_server( const Signature&, const Address& );
         eResult unregister_server( const Signature&, const Address& );
         eResult register_client( const Signature&, const Address& );
         eResult unregister_client( const Signature&, const Address& );

      public:
         const Address::tOpt& server( const Signature& ) const;
         const Address::tSet& clients( const Signature& ) const;

      private:
         std::map< Signature, Connection > m_db;
   };

} // namespace base::interface
