#pragma once

#include "api/sys/common/Types.hpp"
#include "api/sys/helpers/functions/pointer.hpp"



namespace base {



   class StackBuffer
   {
      public:
         StackBuffer( const size_t capacity = 1024, const bool is_reallocate_allowed = true );
         ~StackBuffer( );

      public:
         bool push_back( const void* const buffer, const size_t size, const std::optional< bool > is_reallocate = std::nullopt );
         void pop_back( const size_t size );
         bool back( void* buffer, const size_t size );
         bool front( void* const buffer, const size_t size, const size_t offset = 0 );

      public:
         void dump( ) const;
         void info( ) const;

      public:
         bool allocate( const size_t size );
         bool reallocate( const size_t size, const bool is_store = false );
         bool trancate( );
         void reset( );
         void fill( const char symbol = 0 );

      public:
         const void* const buffer( ) const;
         size_t capacity( ) const;
         size_t size( ) const;

      private:
         void* mp_buffer = nullptr;
         size_t m_capacity = 0;
         size_t m_size = 0;
         bool m_is_reallocate_allowed = true;
   };



   inline
   const void* const StackBuffer::buffer( ) const
   {
      return mp_buffer;
   }

   inline
   size_t StackBuffer::size( ) const
   {
      return m_size;
   }

   inline
   size_t StackBuffer::capacity( ) const
   {
      return m_capacity;
   }

   inline
   void StackBuffer::reset( )
   {
      m_size = 0;   
   }

   inline
   bool StackBuffer::trancate( )
   {
      if( m_size == m_capacity )
         return true;

      return reallocate( m_size, true );
   }

} // namespace base
