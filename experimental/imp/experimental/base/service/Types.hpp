#pragma once

#include <string>



namespace fw::service {

   using tServiceID = std::string;
   using tServiceRole = std::string;
   using tSeqID = std::size_t;

   struct Info
   {
      Info( const tServiceID&, const tServiceRole& );

      bool operator<( const Info& connection ) const;

      const std::string to_string( ) const;

      tServiceID id;
      tServiceRole role;
   };

} // namespace fw::service
