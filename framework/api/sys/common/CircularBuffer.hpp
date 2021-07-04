#pragma once

#include <cstring>
#include <optional>

#include "api/sys/common/RawBuffer.hpp"
#include "api/sys/helpers/functions/pointer.hpp"



namespace base {



   class CircularBuffer
   {
   public:
      enum class eDump { Raw, Logic, All };
      enum class ePush { OK, Realloc, Overlap, Error };
      static const char* c_str( const ePush );

   public:
      CircularBuffer(
                        const std::size_t capacity = 1024,
                        const bool is_overlap_allowed = true,
                        const bool is_reallocate_allowed = false,
                        const bool auto_free = true
                     );
      CircularBuffer( const CircularBuffer& buffer );
      ~CircularBuffer( );

      void auto_free( const bool af );

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

      /******************************************************************************
       *
       * Checks if phisical representation of data is linear in the buffer.
       * Parameter:
       *    pointer - pointer to the buffer will be set as a result if result as true, otherwise - nullptr.
       *    size - size of buffer will be set as a result if result is true, otherwise - 0.
       *
       *****************************************************************************/
      bool is_linear( const void*& pointer, std::size_t& size ) const;
      bool is_linear( ) const;

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
      void*          mp_head = nullptr;
      void*          mp_tail = nullptr;
      void*          mp_begin = nullptr;
      void*          mp_end = nullptr;
      std::size_t    m_capacity = 0;
      std::size_t    m_size = 0;
      const bool     m_is_overlap_allowed = true;
      const bool     m_is_reallocate_allowed = false;
      bool           m_auto_free = true;

   public:
      bool state_save( );
      bool state_restore( );
      bool state_lock( );
      bool state_unlock( );
   private:
      struct state
      {
         struct locker
         {
            locker( bool& _state ) : state( _state ) { state = true; }
            ~locker( ) { state = false; }
         private:
            bool& state;
         };

         void*       mp_begin = nullptr;
         void*       mp_end = nullptr;
         std::size_t      m_size = 0;
      };
      std::optional< state > m_state = std::nullopt;
      bool m_is_state_locked = false;
   };



   inline
   void CircularBuffer::auto_free( const bool af )
   {
      m_auto_free = af;
   }

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

   inline
   bool CircularBuffer::is_linear( ) const
   {
      return m_size <= static_cast< std::size_t >( diff( mp_tail, mp_begin ) );
   }

   inline
   bool CircularBuffer::is_linear( const void*& pointer, std::size_t& size ) const
   {
      if( false == is_linear( ) )
      {
         pointer = nullptr;
         size = 0;
         return false;
      }

      pointer = mp_begin;
      size = m_size;
      return true;
   }

} // namespace base
