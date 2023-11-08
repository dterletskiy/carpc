#pragma once

#include "carpc/common/ByteStream.hpp"
#include "carpc/common/ByteStreamEx.hpp"



namespace carpc::ipc {

   using tStream = carpc::ByteStream;

   template< typename ... TYPES >
   bool serialize( tStream& stream, const TYPES& ... args )
   {
      return stream.push( args... );
   }

   template< typename ... TYPES >
   bool deserialize( tStream& stream, TYPES& ... args )
   {
      return stream.pop( args... );
   }

   using tStreamEx = carpc::ByteStreamEx;

   template< typename ... TYPES >
   bool serialize( tStreamEx& stream, const TYPES& ... args )
   {
      return stream.push( args... );
   }

   template< typename ... TYPES >
   bool deserialize( tStreamEx& stream, TYPES& ... args )
   {
      return stream.pop( args... );
   }

} // namespace carpc
