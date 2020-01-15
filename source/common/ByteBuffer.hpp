#include "Types.hpp"
#include <memory.h>
#include <string.h>

#include "Trace.hpp"
#define CLASS_ABBR "B_BUFFER"

#pragma once



namespace base {



class ByteBuffer
{
public:
   ByteBuffer( );
   ByteBuffer( const size_t );
   ByteBuffer( const uint8_t*, const size_t );
   ~ByteBuffer( );

public:
   bool allocate( const size_t );
   bool reallocate( const size_t, const bool is_store = false );
   bool trancate( );
   void reset( );

public:
   bool push_back( const void*, const size_t, const bool is_reallocate = false );
   bool push_back( const uint8_t*, const size_t, const bool is_reallocate = false );
   bool push_back( const char*, const size_t, const bool is_reallocate = false );
   bool push_back( const std::string&, const bool is_reallocate = false );

   template< typename TYPE >
      typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
         push_back( const TYPE& value, const bool is_reallocate = false )
         {
            return push_back( static_cast< const void* >( &value ), sizeof( TYPE ), is_reallocate );
         }

public:
   bool pop_back( const void*, const size_t );
   bool pop_back( const uint8_t*, const size_t );
   bool pop_back( const char*, const size_t );
   bool pop_back( std::string&, const size_t );

   template< typename TYPE >
      typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
         pop_back( TYPE& value )
         {
            return pop_back( static_cast< const void* >( &value ), sizeof( TYPE ) );
         }

public:
   const uint8_t* const buffer( ) const;
private:
   uint8_t* mp_buffer = nullptr;

public:
   const size_t capacity( ) const;
private:
   size_t   m_capacity = 0;

public:
   const size_t size( ) const;
private:
   size_t   m_size = 0;

public:
   void print( ) const;
   void info( ) const;
};



inline
const uint8_t* const ByteBuffer::buffer( ) const
{
   return mp_buffer;
}

inline
const size_t ByteBuffer::size( ) const
{
   return m_size;
}

inline
const size_t ByteBuffer::capacity( ) const
{
   return m_capacity;
}



} // namespace base

#undef CLASS_ABBR
