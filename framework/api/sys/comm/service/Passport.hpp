#pragma once

#include "api/sys/comm/service/Address.hpp"
#include "api/sys/comm/service/Signature.hpp"



namespace base::service {

   struct Passport
   {
      using tVector = std::vector< Passport >;
      using tList = std::list< Passport >;

      Passport( );
      Passport( const Signature&, const Address& );

      const std::string name( ) const;

      bool to_stream( base::ipc::tStream& ) const;
      bool from_stream( base::ipc::tStream& );

      Signature   signature;
      Address     address;
   };

} // namespace base::service
