#pragma once

#include <set>
#include <map>
#include <memory>



namespace base {

   template< typename tID >
   struct RequestResponseIDs
   {
      tID request = tID::Undefined;
      tID busy = tID::Undefined;
      tID response = tID::Undefined;
   };

   template< typename tID >
   struct NotificationIDs
   {
      tID subscribe = tID::Undefined;
      tID unsubscribe = tID::Undefined;
      tID notification = tID::Undefined;
   };

   template< typename TYPES >
      class TServer;
   template< typename TYPES >
      class TProxy;
   template< typename TYPES >
      class TClient;

} // namespace base
