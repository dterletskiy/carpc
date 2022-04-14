#pragma once

#include "api/sys/comm/Types.hpp"
#include "api/sys/helpers/macros/enum.hpp"



namespace carpc::service {

   class IPC;
   class NO_IPC;

   DEFINE_ENUMERATION( eType, std::uint8_t,
      REQUEST,
      BUSY,
      RESPONSE,
      SUBSCRIBE,
      UNSUBSCRIBE,
      NOTIFICATION
   );



   DEFINE_ENUMERATION( eError, std::uint8_t, RequestBusy, EmptyData );



   template< typename _ID >
   struct RequestResponseIDs
   {
      using tRequestResponseIDs = RequestResponseIDs< _ID >;
      using tVector = std::vector< tRequestResponseIDs >;

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
      using tNotificationIDs = NotificationIDs< _ID >;
      using tVector = std::vector< tNotificationIDs >;

      _ID subscribe = _ID::Undefined;
      _ID unsubscribe = _ID::Undefined;
      _ID notification = _ID::Undefined;
   };

} // namespace carpc::service
