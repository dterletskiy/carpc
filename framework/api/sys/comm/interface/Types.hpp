#pragma once

#include <set>
#include <map>
#include <memory>
#include "api/sys/common/ByteBufferT.hpp"



namespace base {

   template< typename _ID >
   struct RequestResponseIDs
   {
      using tRequestResponseIDs = RequestResponseIDs< _ID >;

      struct Comparator
      {
         struct ByRequest
         {
            bool operator( )( const tRequestResponseIDs& rr1, const tRequestResponseIDs& rr2 ) const
            {
               return rr1.request < rr2.request;
            }
         };
      };

      RequestResponseIDs( const _ID& _request )
         : request( _request ) { }
      RequestResponseIDs( const _ID& _request, const _ID& _busy, const _ID& _response )
         : request( _request ), busy( _busy ), response( _response ) { }

      _ID request = _ID::Undefined;
      _ID busy = _ID::Undefined;
      _ID response = _ID::Undefined;
   };

   template< typename _ID >
   struct NotificationIDs
   {
      _ID subscribe = _ID::Undefined;
      _ID unsubscribe = _ID::Undefined;
      _ID notification = _ID::Undefined;
   };

} // namespace base
