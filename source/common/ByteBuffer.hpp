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
   template< typename TYPE >
      ByteBuffer& operator << ( const TYPE& );
   template< typename TYPE >
      ByteBuffer& operator >> ( TYPE& );

public:
   ByteBuffer( );
   ByteBuffer( const void*, const size_t );
   template< typename TYPE >
      ByteBuffer( const TYPE& );
   ~ByteBuffer( );

public:
   bool allocate( const size_t );
   bool reallocate( const size_t, const bool is_store = false );
   bool trancate( );
   void reset( );

public:
   bool push( const void*, const size_t, const bool is_reallocate = false );
   bool push( const std::string&, const bool is_reallocate = false );

   template< typename TYPE >
      typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
         push( const TYPE&, const bool is_reallocate = false );

public:
   bool pop( const void*, const size_t );
   bool pop( std::string& );

   template< typename TYPE >
      typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
         pop( TYPE& );

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
   void dump( ) const;
   void info( ) const;
};



template< typename TYPE >
ByteBuffer::ByteBuffer( const TYPE& data )
{
   if( false == allocate( sizeof( data ) ) ) return;
   *this << data;
}

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

template< typename TYPE >
ByteBuffer& ByteBuffer::operator << ( const TYPE& value )
{
   push( value, true );
   return *this;
}

template< typename TYPE >
ByteBuffer& ByteBuffer::operator >> ( TYPE& value )
{
   pop( value );
   return *this;
}

template< typename TYPE >
typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
ByteBuffer::push( const TYPE& value, const bool is_reallocate )
{
   return push( static_cast< const void* >( &value ), sizeof( TYPE ), is_reallocate );
}

template< typename TYPE >
typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
ByteBuffer::pop( TYPE& value )
{
   return pop( static_cast< const void* >( &value ), sizeof( TYPE ) );
}



} // namespace base

#undef CLASS_ABBR
