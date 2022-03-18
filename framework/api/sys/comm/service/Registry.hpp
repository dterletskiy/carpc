#pragma once

#include "api/sys/comm/service/Address.hpp"
#include "api/sys/comm/service/Signature.hpp"
#include "api/sys/comm/service/Passport.hpp"



namespace carpc::service {

   class Registry
   {
      public:
         enum class eResult { OK_NotPaired, OK_Paired, Duplicate, NotFound, Error };

      public:
         Registry( ) = default;
         ~Registry( ) = default;
      private:
         Registry( const Registry& ) = delete;
         Registry& operator=( const Registry& ) = delete;

      public:
         eResult register_server( const Signature&, const Address& );
         eResult unregister_server( const Signature&, const Address& );
         eResult register_client( const Signature&, const Address& );
         eResult unregister_client( const Signature&, const Address& );
      public:
         eResult register_server( const Passport& );
         eResult unregister_server( const Passport& );
         eResult register_client( const Passport& );
         eResult unregister_client( const Passport& );

      public:
         const Address& server( const Signature& ) const;
         const Address::tSet& clients( const Signature& ) const;
      public:
         const Address& server( const Passport& ) const;
         const Address::tSet& clients( const Passport& ) const;
   };

} // namespace carpc::service
