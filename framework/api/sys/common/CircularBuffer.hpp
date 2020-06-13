#pragma once

#include "api/sys/common/Types.hpp"


namespace base {



class CircularBuffer
{
public:
   enum class eDump { Raw, Logic, All };
   enum class ePush { OK, Realloc, Overlap, Error };
   static const char* c_str( const ePush );

   struct raw_buffer
   {
      void* ptr = nullptr;
      size_t size = 0;
   };

public:
   CircularBuffer( const size_t capacity = 1024, const bool is_overlap_allowed = true, const bool is_reallocate_allowed = false );
   ~CircularBuffer( );

public:
   /******************************************************************************
    *
    * Dump buffer content.
    * Parameters:
    *    eDump::Raw - raw representation.
    *    eDump::Logic - logical representation.
    *    eDump::All - both representations.
    *
    *****************************************************************************/
   void dump( const eDump type = eDump::All ) const;

   /******************************************************************************
    *
    * Pring buffer info.
    *
    *****************************************************************************/
   void info( ) const;

public:
   /******************************************************************************
    *
    * Reallocate buffer with moving data.
    * Parameters:
    *    capacity - new buffer capacity. If capacity < m_size some data will be lost.
    *
    *****************************************************************************/
   bool reallocate( size_t capacity = 0 );

   /******************************************************************************
    *
    * Trancate buffer to current size value.
    *
    *****************************************************************************/
   bool trancate( );

   /******************************************************************************
    *
    * Adding new data to the end.
    * Parameters:
    *    buffer - source buffer pointer.
    *    size - source data size.
    *    is_reallocate - indicates is buffer reallocation is allowed in case of not enough free space and overlapping is not allowed.
    *                    In case this parameter is nullopt m_is_reallocate_allowed value will be used.
    * Return value:
    *    ePush::OK - operation success and without overlapping.
    *    ePush::Realloc - operation success but with reallocation and without overlapping.
    *    ePush::Overlap - operation success and with overlapping.
    *    ePush::Error - operation error.
    *
    *****************************************************************************/
   ePush push_back( const void* const buffer, const size_t size, const std::optional< bool > is_reallocate = std::nullopt );
   ePush push_back( const raw_buffer& buffer, const std::optional< bool > is_reallocate = std::nullopt );

   /******************************************************************************
    *
    * Removing data from the beginning.
    * Parameters:
    *    size - size of data to be removed. If this parameter more then current data size => all data will be removed.
    *
    *****************************************************************************/
   void pop_front( const size_t size );

   /******************************************************************************
    *
    * Removing data from the end.
    * Parameters:
    *    size - size of data to be removed. If this parameter more then current data size => all data will be removed.
    *
    *****************************************************************************/
   void pop_back( const size_t size );

   /******************************************************************************
    *
    * Copying data to the buffer.
    * Parameters:
    *    buffer - external buffer pointer.
    *    size - size of data to be read.
    *
    *****************************************************************************/
   bool front( const void* buffer, const size_t size ) const;
   bool front( raw_buffer& buffer ) const;

   /******************************************************************************
    *
    * Moving data to the buffer.
    * Equivalent to:
    *    from( buffer, size );
    *    pop_front( size );
    *
    *****************************************************************************/
   bool move_front( const void* const buffer, const size_t size );

   /******************************************************************************
    *
    * Compare buffer content.
    * Parameters:
    *    - buffer - pointer to external buffer with data to compare.
    *    - size - size of external buffer with data to compare.
    *    - offset - offset from data beginning in current curcular buffer to compare.
    *
    *****************************************************************************/
   bool cmp( void* const buffer, const size_t size, const size_t offset = 0 ) const;

   /******************************************************************************
    *
    * Ereasing all data.
    *
    *****************************************************************************/
   void reset( );

   /******************************************************************************
    *
    * Filling whole buffer by some value.
    * Parameters:
    *    symbol - value to be filled.
    *
    *****************************************************************************/
   void fill( const char symbol = 0 ) const;

public:
   size_t size( ) const;
   size_t capacity( ) const;
   size_t free_space( ) const;

private:
   void dump_raw( ) const;
   void dump_logic( ) const;

private:
   void*       mp_head = nullptr;
   void*       mp_tail = nullptr;
   void*       mp_begin = nullptr;
   void*       mp_end = nullptr;
   size_t      m_capacity = 0;
   size_t      m_size = 0;
   const bool  m_is_overlap_allowed = true;
   const bool  m_is_reallocate_allowed = false;
};



inline
bool CircularBuffer::trancate( )
{
   return reallocate( m_size );
}

inline
void CircularBuffer::reset( )
{
   mp_begin = mp_end = mp_head;
   m_size = 0;
}

inline
void CircularBuffer::fill( const char symbol ) const
{
   memset( mp_head, symbol, m_capacity );
}

inline
size_t CircularBuffer::size( ) const
{
   return m_size;
}

inline
size_t CircularBuffer::capacity( ) const
{
   return m_capacity;
}

inline
size_t CircularBuffer::free_space( ) const
{
   return m_capacity - m_size;
}



} // namespace base
