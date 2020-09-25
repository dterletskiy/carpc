#pragma once

#include "api/sys/comm/event/TEvent.hpp"
#include "api/sys/comm/event/TSignature.hpp"



namespace base::async {

   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
   class TGenerator
   {
      using _Generator = TGenerator< _ServiceType, _EventNamespace, _DataType, _SignatureType >;

      public:
         struct Config
         {
            using tEvent            = TEvent< _Generator >;
            using tSignature        = TSignature< _Generator >;
            using tData             = _DataType;
            using tConsumer         = IEvent::TConsumer< tEvent >;
            using tService          = _ServiceType;
            using tProcessor        = void ( tConsumer::* )( const tEvent& );
            using tUserSignature    = _SignatureType;
         };
   };

} // namespace base::async
