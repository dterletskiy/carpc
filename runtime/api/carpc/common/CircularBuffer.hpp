#pragma once

#include <cstring>
#include <optional>

#include "carpc/common/RawBuffer.hpp"
#include "carpc/helpers/functions/pointer.hpp"



namespace carpc {



   /*********************************************************************************************************************************
    *
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   8   |  10   |  11   |  12   |  13   |
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *
    *
    *
    *
    *
    *      head                                                                                                     tail
    *        |                                                                                                       |
    *        |                                                                                                       |
    *        |                                                                                                       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    |   0   |   1   |   2   |   3   |   4   |   5   |   6   |       |       |       |       |       |       |   X   |
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *        |                                                       |
    *        |                                                       |
    *        |                                                       |
    *      begin                                                    end
    *
    *
    *
    *      head                                                                                                     tail
    *        |                                                                                                       |
    *        |                                                                                                       |
    *        |                                                                                                       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    |       |       |   0   |   1   |   2   |   3   |   4   |   5   |       |       |       |       |       |   X   |
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *                        |                                               |
    *                        |                                               |
    *                        |                                               |
    *                      begin                                            end
    *
    *
    *
    *      head                                                                                                     tail
    *        |                                                                                                       |
    *        |                                                                                                       |
    *        |                                                                                                       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    |   5   |   6   |       |       |       |       |       |       |   0   |   1   |   2   |   3   |   4   |   X   |
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *                        |                                               |                                       
    *                        |                                               |                                       
    *                        |                                               |                                       
    *                       end                                            begin
    *
    *
    *
    *      head                                                                                                     tail
    *        |                                                                                                       |
    *        |                                                                                                       |
    *        |                                                                                                       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    |   5   |   6   |   7   |   8   |   9   |  10   |  11   |  12   |   0   |   1   |   2   |   3   |   4   |   X   |
    *    |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
    *    +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
    *                                                                        |                                       
    *                                                                        |                                       
    *                                                                        |                                       
    *                                                                    end / begin
    *
    */



   class CircularBuffer
   {
      public:
         enum class eDump { Raw, Logic, All };
         enum class ePush { OK, Realloc, Overlap, Error };
         static const char* c_str( const ePush );

      public:
         /******************************************************************************
          *
          * Constructor.
          * Parameters:
          *    capacity - allocated memory size.
          *    is_overlap_allowed - overwrite stored data in case if not enough free allocated space.
          *    is_reallocate_allowed - reallocate buffer with more size in case if not enough
          *       free allocated space and overlap is not allowed.
          *       In case if overlap is allowed new data will overwrite previously stored data.
          *       In case if both 'is_overlap_allowed' and 'is_reallocate_allowed' are true
          *       reallocation will be performed only in case when you try to store data with size
          *       more then current buffer capacity.
          *    auto_free - automatically free allocated memory when 'CircularBuffer' is destroyed.
          *    auto_zeroing - automatically fill memory region after poped data by zeroes.
          * 
          * It this contructor is possible to create nnext buffer types:
          *    The circular extendable buffer:
          *       CircularBuffer( capacity, true, true )
          *    The circular not extendable buffer:
          *       CircularBuffer( capacity, true, false )
          *    The liner extendable buffer:
          *       CircularBuffer( capacity, false, true )
          *    The liner not extendable buffer:
          *       CircularBuffer( capacity, false, false )
          *
          *****************************************************************************/
         CircularBuffer(
                           const std::size_t capacity = 1024,
                           const bool is_overlap_allowed = true,
                           const bool is_reallocate_allowed = false,
                           const bool auto_free = true,
                           const bool auto_zeroing = false
                        );
         CircularBuffer( const CircularBuffer& other );
         CircularBuffer( CircularBuffer&& other );
         ~CircularBuffer( );

         void auto_free( const bool af );
         void auto_zeroing( const bool az );

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
         bool reallocate( std::size_t capacity = 0 );

         /******************************************************************************
          *
          * Checks if phisical representation of data in the memory is linear in the buffer.
          *
          *****************************************************************************/
         bool is_linear( ) const;

         /******************************************************************************
          *
          * Trancate buffer to current size value.
          * This operation will performe memory reallocation for new buffer with capacity
          * equal to size.
          *
          *****************************************************************************/
         // bool trancate( );

         /******************************************************************************
          *
          * Checks if phisical representation of data is linear in the buffer and
          * in case of not it will be realocated with linear representation of data in the memory.
          * Parameters:
          *    trancate - flag indicates is new buffer will be trancated to current size.
          *
          *****************************************************************************/
         bool normalize( const bool trancate = false );

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
         ePush push_back( const void* const buffer, const std::size_t size, const std::optional< bool > is_reallocate = std::nullopt );
         ePush push_back( const RawBuffer& buffer, const std::optional< bool > is_reallocate = std::nullopt );
         ePush push_back( const CircularBuffer& buffer, const std::optional< bool > is_reallocate = std::nullopt );

         /******************************************************************************
          *
          * Removing data from the beginning.
          * Parameters:
          *    size - size of data to be removed. If this parameter more then current data size => all data will be removed.
          *
          *****************************************************************************/
         void pop_front( const std::size_t size );

         /******************************************************************************
          *
          * Removing data from the end.
          * Parameters:
          *    size - size of data to be removed. If this parameter more then current data size => all data will be removed.
          *
          *****************************************************************************/
         void pop_back( const std::size_t size );

         /******************************************************************************
          *
          * Copying data from offset position to the buffer.
          * Parameters:
          *    buffer - external buffer pointer.
          *    size - size of data to be read.
          *
          *****************************************************************************/
         bool get( void* const buffer, const std::size_t size, const std::size_t offset = 0 ) const;

         /******************************************************************************
          *
          * Copying data from begining to the buffer.
          * Parameters:
          *    buffer - external buffer pointer.
          *    size - size of data to be read.
          *
          *****************************************************************************/
         bool front( void* const buffer, const std::size_t size ) const;
         bool front( RawBuffer& buffer ) const;
         bool front( CircularBuffer& buffer ) const;

         /******************************************************************************
          *
          * Moving data from begining to the buffer.
          * Equivalent to:
          *    from( buffer, size );
          *    pop_front( size );
          *
          *****************************************************************************/
         bool move_front( void* const buffer, const std::size_t size );
         bool move_front( RawBuffer& buffer );
         bool move_front( CircularBuffer& buffer );

         /******************************************************************************
          *
          * Compare buffer content.
          * Parameters:
          *    - buffer - pointer to external buffer with data to compare.
          *    - size - size of external buffer with data to compare.
          *    - offset - offset from data beginning in current circular buffer to compare.
          *
          *****************************************************************************/
         bool cmp( void* const buffer, const std::size_t size, const std::size_t offset = 0 ) const;
         bool cmp( const RawBuffer& buffer, const std::size_t offset = 0 ) const;

         /******************************************************************************
          *
          * Find buffer content.
          * Parameters:
          *    - buffer - pointer to external buffer with data to compare.
          *    - size - size of external buffer with data to compare.
          *    - offset - offset from data beginning in current circular buffer to find.
          *
          *****************************************************************************/
         ssize_t find( void* const buffer, const std::size_t size, const std::size_t offset = 0 ) const;
         ssize_t find( const RawBuffer& buffer, const std::size_t offset = 0 ) const;

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
         std::size_t size( ) const;
         std::size_t capacity( ) const;
         std::size_t free_space( ) const;
         const void* const ptr( ) const;
         const RawBuffer buffer( ) const;

      private:
         void dump_raw( ) const;
         void dump_logic( ) const;

      private:
         void*          mp_head = nullptr;      // start of allocated buffer
         void*          mp_tail = nullptr;      // end of allocated buffer
         void*          mp_begin = nullptr;     // start of data
         void*          mp_end = nullptr;       // end of data + 1
         std::size_t    m_capacity = 0;
         std::size_t    m_size = 0;
         const bool     m_is_overlap_allowed = true;
         const bool     m_is_reallocate_allowed = false;
         bool           m_auto_free = true;
         bool           m_auto_zeroing = false;
   };



   inline
   void CircularBuffer::auto_free( const bool af )
   {
      m_auto_free = af;
   }

   inline
   void CircularBuffer::auto_zeroing( const bool az )
   {
      m_auto_zeroing = az;
   }

   inline
   bool CircularBuffer::is_linear( ) const
   {
      return m_size <= static_cast< std::size_t >( diff( mp_tail, mp_begin ) );
   }

   // inline
   // bool CircularBuffer::trancate( )
   // {
   //    if( 0 == free_space( ) )
   //       return false;

   //    return reallocate( m_size );
   // }

   inline
   void CircularBuffer::reset( )
   {
      mp_begin = mp_head;
      mp_end = mp_head;
      m_size = 0;

      if( m_auto_free )
      {
         fill( );
      }
   }

   inline
   void CircularBuffer::fill( const char symbol ) const
   {
      std::memset( mp_head, symbol, m_capacity );
   }

   inline
   std::size_t CircularBuffer::size( ) const
   {
      return m_size;
   }

   inline
   std::size_t CircularBuffer::capacity( ) const
   {
      return m_capacity;
   }

   inline
   std::size_t CircularBuffer::free_space( ) const
   {
      return m_capacity - m_size;
   }

   inline
   const void* const CircularBuffer::ptr( ) const
   {
      return mp_head;
   }

   inline
   const RawBuffer CircularBuffer::buffer( ) const
   {
      return { mp_head, m_size };
   }

} // namespace carpc
