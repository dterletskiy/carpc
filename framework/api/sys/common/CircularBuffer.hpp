#pragma once

#include "api/sys/common/Types.hpp"


namespace base {



class CircularBuffer
{
public:
   enum class eDump { Raw, Logic, All };

public:
   CircularBuffer( const size_t _capacity = 1024 );
   ~CircularBuffer( );

public:
   void dump( const eDump type = eDump::All ) const;
private:
   void dump_raw( ) const;
   void dump_logic( ) const;

public:
   bool insert_data( void* const p_buffer, const size_t size );
   bool move_data( void* const p_buffer, const size_t size );
   bool copy_data( void* const p_buffer, const size_t size ) const;

public:
   size_t size( ) const;
   size_t capacity( ) const;
   size_t free_space( ) const;
private:
   void*    mp_head = nullptr;
   void*    mp_tail = nullptr;
   void*    mp_begin = nullptr;
   void*    mp_end = nullptr;
   size_t   m_capacity = 0;
   size_t   m_size = 0;
};



size_t CircularBuffer::size( ) const
{
   return m_size;
}

size_t CircularBuffer::capacity( ) const
{
   return m_capacity;
}

size_t CircularBuffer::free_space( ) const
{
   return m_capacity - m_size;
}



} // namespace base
