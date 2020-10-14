#pragma once

#include "api/sys/common/Includes.hpp"
#include "api/sys/comm/service/Address.hpp"
#include "api/sys/comm/service/Signature.hpp"



namespace base::service {

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
         const Address& server( const Signature& ) const;
         const Address::tSet& clients( const Signature& ) const;
   };

} // namespace base::service
