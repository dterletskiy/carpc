#pragma once

#include "api/sys/common/ByteStream.hpp"



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



   inline
   bool append( tStream& stream, const void* const buffer, const std::size_t size )
   {
      return stream.push( buffer, size );
   }

   inline
   bool remove( tStream& stream, void* const buffer, const std::size_t size )
   {
      return stream.pop( buffer, size );
   }



   inline
   bool raw_buffer( const tStream& stream, RawBuffer& raw_buffer )
   {
      return stream.is_linear( raw_buffer );
   }

   inline
   bool raw_buffer( const tStream& stream, void*& buffer, std::size_t& size )
   {
      return stream.is_linear( buffer, size );
   }



   // template< typename ... TYPES >
   // bool serialize( RawBuffer& raw_buffer, const TYPES& ... args )
   // {
   //    return tStream::serialize( raw_buffer, args... );
   // }

   // template< typename ... TYPES >
   // bool deserialize( RawBuffer& raw_buffer, TYPES& ... args )
   // {
   //    return tStream::deserialize( raw_buffer, args... );
   // }

} // namespace carpc



namespace carpc::experimental {

   class ipc
   {
      public:
         using tStream = ipc;
      private:
         using _tStream_ = carpc::ByteStream;

      public:
         ipc( ) = default;
         ~ipc( ) = default;

      public:
         template< typename ... TYPES >
         static bool serialize( tStream& stream, const TYPES& ... args )
         {
            return stream.m_stream.push( args... );
         }

         template< typename ... TYPES >
         static bool deserialize( tStream& stream, TYPES& ... args )
         {
            return stream.m_stream.pop( args... );
         }

      public:
         bool append( tStream& stream, const void* const buffer, const std::size_t size )
         {
            return stream.m_stream.push( buffer, size );
         }

         bool remove( tStream& stream, void* const buffer, const std::size_t size )
         {
            return stream.m_stream.pop( buffer, size );
         }

      public:
         bool raw_buffer( tStream& stream, RawBuffer& raw_buffer )
         {
            return stream.m_stream.is_linear( raw_buffer );
         }

         bool raw_buffer( tStream& stream, void*& buffer, std::size_t& size )
         {
            return stream.m_stream.is_linear( buffer, size );
         }

      private:
         _tStream_ m_stream;
   };

} // namespace carpc

