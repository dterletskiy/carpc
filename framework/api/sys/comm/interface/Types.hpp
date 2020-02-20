#pragma once

#include <memory>



namespace base {

   template< typename tID >
   struct RequestResponse
   {
      tID request = tID::Undefined;
      tID busy = tID::Undefined;
      tID response = tID::Undefined;
   };

   template< typename tID >
   struct Notification
   {
      tID subscribe = tID::Undefined;
      tID unsubscribe = tID::Undefined;
      tID notification = tID::Undefined;
   };

} // namespace base
