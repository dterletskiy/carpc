#pragma once

#include <memory>



namespace base {

   template< typename tID >
   struct RequestResponse
   {
      tID request;
      tID busy;
      tID response = tID::Undefined;
   };

} // namespace base
