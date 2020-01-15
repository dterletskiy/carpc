#include "Types.hpp"
#include <memory.h>
#include <string.h>

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
   bool add( const uint8_t*, const size_t, const bool is_reallocate = false );
   bool add( const char*, const size_t, const bool is_reallocate = false );
   bool add( const std::string&, const bool is_reallocate = false );

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
