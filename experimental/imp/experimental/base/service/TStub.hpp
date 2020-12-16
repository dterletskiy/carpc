#pragma once

#include "imp/experimental/base/service/IStub.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TStub"



namespace fw::service {

   template< typename T >
   class TStub : public IStub
   {
      public:
         TStub( const tServiceID&, const tServiceRole& );
         ~TStub( ) override;



   template< typename T >
   TStub< T >::TStub( const tServiceID& id, const tServiceRole& role )
      : IProxy( id, role )
   {
   }

   template< typename T >
   TStub< T >::~TStub( )
   {
   }

} // namespace fw::service
