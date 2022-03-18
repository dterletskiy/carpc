#pragma once

#include "api/sys/comm/async/event/TEvent.hpp"
#include "api/sys/comm/async/event/TSignature.hpp"
#include "api/sys/comm/async/event/TConsumer.hpp"



namespace carpc::async {

   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
   class TGenerator
   {
      using _Generator = TGenerator< _ServiceType, _EventNamespace, _DataType, _SignatureType >;

      public:
         struct Config
         {
            using tEvent            = TEvent< _Generator >;
            using tSignature        = TSignature< _Generator >;
            using tConsumer         = TConsumer< _Generator >;
            using tData             = _DataType;
            using tService          = _ServiceType;
            using tProcessor        = void ( tConsumer::* )( const tEvent& );
            using tUserSignature    = _SignatureType;
         };
   };

} // namespace carpc::async
