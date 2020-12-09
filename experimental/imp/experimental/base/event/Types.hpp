#pragma once

#include <string>



namespace fw::event {

   using tClassID = std::string;
   using tID = std::size_t;

   struct Info
   {
      bool operator<( const Info& ) const;

      const std::string to_string( ) const;

      tClassID class_id;
      tID      id;
   };

} // namespace fw::event
