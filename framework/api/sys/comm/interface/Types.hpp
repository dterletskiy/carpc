#pragma once

#include <set>
#include <map>
#include <memory>



namespace base {

   template< typename tID >
   struct RequestResponseIDs
   {
      RequestResponseIDs( const tID& _request )
         : request( _request ) { }
      RequestResponseIDs( const tID& _request, const tID& _busy, const tID& _response )
         : request( _request ), busy( _busy ), response( _response ) { }

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
