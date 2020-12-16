#pragma once

#include "imp/experimental/base/service/IProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TProxy"



namespace fw::service {

   template< typename T >
   class TProxy : public IProxy
   {
      public:
         TProxy( const tServiceID&, const tServiceRole& );
         ~TProxy( ) override;
   };



   template< typename T >
   TProxy< T >::TProxy( const tServiceID& id, const tServiceRole& role )
      : IProxy( id, role )
   {
   }

   template< typename T >
   TProxy< T >::~TProxy( )
   {
   }

} // namespace fw::service



#undef CLASS_ABBR
