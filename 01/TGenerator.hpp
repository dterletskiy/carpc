#pragma once

#include "carpc/base/helpers/macros/types.hpp"
#include "carpc/runtime/comm/async/event/TEvent.hpp"
#include "carpc/runtime/comm/async/event/TSignature.hpp"
#include "carpc/runtime/comm/async/event/TConsumer.hpp"
#include "carpc/runtime/comm/async/event/TData.hpp"



namespace carpc::async {

   // Base Event Generator
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
   class TGeneratorBase
   {
      using _Generator = TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _SignatureType >;

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



   // Generic Event Generator
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType, typename _Activator = void >
   class TGenerator
      : public TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _SignatureType >
   {
   };

   namespace generic {

      template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
         using tGenerator =
            TGenerator< _ServiceType, _EventNamespace, _DataType, _SignatureType, void >;

   }



   // Event Generator with user signature type = integral value
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
   class TGenerator< _ServiceType, _EventNamespace, _DataType, _SignatureType, CARPC_ENABLE_IF_INTEGRAL_TYPE( _SignatureType ) >
      : public TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _SignatureType >
   {
   };

   namespace integral {

      template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
         using tGenerator =
            TGenerator< _ServiceType, _EventNamespace, _DataType, _SignatureType, CARPC_ENABLE_IF_INTEGRAL_TYPE( _SignatureType ) >;

   }



   // Event Generator with user signature type = enum
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
   class TGenerator< _ServiceType, _EventNamespace, _DataType, _SignatureType, CARPC_ENABLE_IF_ENUM_TYPE( _SignatureType ) >
      : public TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _SignatureType >
   {
   };

   namespace enumeration {

      template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _SignatureType >
         using tGenerator =
            TGenerator< _ServiceType, _EventNamespace, _DataType, _SignatureType, CARPC_ENABLE_IF_ENUM_TYPE( _SignatureType ) >;

   }

} // namespace carpc::async
