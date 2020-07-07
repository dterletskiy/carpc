#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/common/CircularBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "C_BUFFER"



using namespace base;



const char* CircularBuffer::c_str( const ePush result )
{
   switch( result )
   {
      case ePush::OK:         return "ePush::OK";
      case ePush::Realloc:    return "ePush::Realloc";
      case ePush::Overlap:    return "ePush::Overlap";
      case ePush::Error:      return "ePush::Error";
   }
   return "ePush::XXX";
}

CircularBuffer::CircularBuffer( const size_t capacity, const bool is_overlap_allowed, const bool is_reallocate_allowed, const bool auto_free )
   : m_capacity( capacity )
   , m_is_overlap_allowed( is_overlap_allowed )
   , m_is_reallocate_allowed( is_reallocate_allowed )
   , m_auto_free( auto_free )
{
   mp_head = malloc( m_capacity + 1 );
   mp_tail = inc( mp_head, m_capacity );
   mp_begin = mp_end = mp_head;
}

CircularBuffer::CircularBuffer( const CircularBuffer& cb )
   : m_capacity( cb.m_capacity )
   , m_is_overlap_allowed( cb.m_is_overlap_allowed )
   , m_is_reallocate_allowed( cb.m_is_reallocate_allowed )
   , m_auto_free( cb.m_auto_free )
{
   mp_head = malloc( m_capacity + 1 );
   mp_tail = inc( mp_head, m_capacity );
   mp_begin = mp_end = mp_head;

   cb.front( mp_head, cb.size( ) );
   mp_end = inc( mp_begin, cb.size( ) );

   m_size = cb.m_size;
}

CircularBuffer::~CircularBuffer( )
{
   if( true == m_auto_free )
      free( mp_head );
}

bool CircularBuffer::reallocate( size_t capacity )
{
   if( std::nullopt != m_state )
   {
      SYS_ERR( "restore saved state" );
      return false;
   }
   state::locker locker( m_is_state_locked );

   if( 0 == capacity )
      capacity = 1.5 * m_capacity;

   void* p_head = malloc( capacity + 1 );
   if( nullptr == p_head )
      return false;

   size_t size = std::min( capacity, m_size );
   front( p_head, size );
   free( mp_head );
   m_capacity = capacity;
   m_size = size;
   mp_head = p_head;
   mp_tail = inc( mp_head, m_capacity );
   mp_begin = mp_head;
   mp_end = inc( mp_begin, m_size );
   if( mp_tail == mp_end )
      mp_end = mp_head;

   return true;
}

void CircularBuffer::info( ) const
{
   SYS_INF( "head: %p / tail: %p", mp_head, mp_tail );
   SYS_INF( "begin: %p / end: %p", mp_begin, mp_end );
   SYS_INF( "capacity: %zu / size: %zu", m_capacity, m_size );
   SYS_INF( "linear: %s", BOOL_TO_STRING( is_linear( ) ) );
   SYS_INF( "overlapping: %s", BOOL_TO_STRING( m_is_overlap_allowed ) );
   SYS_INF( "reallocate: %s", BOOL_TO_STRING( m_is_reallocate_allowed ) );
}

void CircularBuffer::dump( const eDump type ) const
{
   SYS_INF( "--------------- Dump begin ---------------" );
   info( );
   switch( type )
   {
      case eDump::Raw:
      {
         dump_raw( );
         break;
      }
      case eDump::Logic:
      {
         dump_logic( );
         break;
      }
      case eDump::All:
      {
         dump_raw( );
         dump_logic( );
         break;
      }
   }
   SYS_INF( "---------------- Dump end ----------------" );
}

void CircularBuffer::dump_raw( ) const
{
   printf( "Raw dump: " );
   for( size_t i = 0; i < m_capacity; ++i )
      printf( "%#x ", static_cast< uint8_t* >( mp_head )[i] );

   printf( "\n" );
}

void CircularBuffer::dump_logic( ) const
{
   printf( "Logic dump: " );
   const size_t part_size = static_cast< size_t >( diff( mp_tail, mp_begin ) );
   if( m_size > part_size )
   {
      for( size_t i = 0; i < part_size; ++i )
         printf( "%#x ", static_cast< uint8_t* >( mp_begin )[i] );
      for( size_t i = 0; i < m_size - part_size; ++i )
         printf( "%#x ", static_cast< uint8_t* >( mp_head )[i] );
   }
   else
   {
      for( size_t i = 0; i < m_size; ++i )
         printf( "%#x ", static_cast< uint8_t* >( mp_begin )[i] );
   }

   printf( "\n" );
}

CircularBuffer::ePush CircularBuffer::push_back( const void* const p_buffer, const size_t size, const std::optional< bool > is_reallocate )
{
   if( std::nullopt != m_state )
   {
      SYS_ERR( "restore saved state" );
      return ePush::Error;
   }
   state::locker locker( m_is_state_locked );

   if( nullptr == p_buffer )
   {
      SYS_ERR( "source buffer pointer is nullptr" );
      return ePush::Error;
   }
   if( size > m_capacity && false == is_reallocate.value_or( m_is_reallocate_allowed ) )
   {
      SYS_ERR( "buffer capacity(%zu) not enough", m_capacity );
      return ePush::Error;
   }

   ePush result = ( size > free_space( ) ) ? ePush::Overlap : ePush::OK;
   if( ePush::Overlap == result && false == m_is_overlap_allowed )
   {
      if( false == is_reallocate.value_or( m_is_reallocate_allowed ) )
         return ePush::Error;

      if( false == reallocate( 1.5 * ( m_size + size ) ) )
         return ePush::Error;
      result = ePush::Realloc;
   }

   const size_t part_size = static_cast< size_t >( diff( mp_tail, mp_end ) );
   if( size > part_size )
   {
      memcpy( mp_end, p_buffer, part_size );
      memcpy( mp_head, inc( p_buffer, part_size ), size - part_size );
      mp_end = inc( mp_head, size - part_size );
   }
   else
   {
      memcpy( mp_end, p_buffer, size );
      mp_end = inc( mp_end, size );
   }

   if( mp_end == mp_tail )
      mp_end = mp_head;

   if( size > free_space( ) )
   {
      m_size = m_capacity;
      mp_begin = mp_end;
   }
   else
   {
      m_size += size;
   }

   return result;
}

CircularBuffer::ePush CircularBuffer::push_back( const RawBuffer& buffer, const std::optional< bool > is_reallocate )
{
   return push_back( buffer.ptr, buffer.size, is_reallocate );
}

CircularBuffer::ePush CircularBuffer::push_back( const CircularBuffer& buffer, const std::optional< bool > is_reallocate )
{
   const size_t part_size = static_cast< size_t >( diff( buffer.mp_tail, buffer.mp_begin ) );
   ePush result = ePush::Error;
   if( part_size < buffer.m_size )
   {
      result = push_back( buffer.mp_begin, part_size, is_reallocate );
      if( ePush::Error == result )
         return result;
      result = push_back( buffer.mp_head, buffer.m_size - part_size, is_reallocate );
      if( ePush::Error == result )
         return result;
   }
   else
   {
      result = push_back( buffer.mp_begin, buffer.m_size, is_reallocate );
      if( ePush::Error == result )
         return result;
   }

   return result;
}

void CircularBuffer::pop_front( const size_t size )
{
   state::locker locker( m_is_state_locked );

   if( 0 == m_size )
      return;

   if( size >= m_size )
      return reset( );

   const size_t part_size = static_cast< size_t >( diff( mp_tail, mp_begin ) );
   if( size > part_size )
   {
      // zeroing in case if context was not save => will not be restored in other case modification is not allowed
      // (should be after updating mp_end)
      if( std::nullopt == m_state )
      {
         memset( mp_begin, 0, part_size );
         memset( mp_head, 0, size - part_size );
      }

      mp_begin = inc( mp_head, size - part_size );
   }
   else
   {
      // zeroing in case if context was not save => will not be restored in other case modification is not allowed
      // (should be after updating mp_end)
      if( std::nullopt == m_state )
         memset( mp_begin, 0, size );

      mp_begin = inc( mp_begin, size );
   }

   if( mp_begin == mp_tail )
      mp_begin = mp_head;

   m_size -= size;
}

void CircularBuffer::pop_back( const size_t size )
{
   state::locker locker( m_is_state_locked );

   if( 0 == m_size )
      return;

   if( size >= m_size )
      return reset( );

   const size_t part_size = static_cast< size_t >( diff( mp_head, mp_end ) );
   if( size > part_size )
   {
      mp_end = dec( mp_tail, size - part_size );

      // zeroing in case if context was not save => will not be restored in other case modification is not allowed
      // (should be after updating mp_end)
      if( std::nullopt == m_state )
      {
         memset( mp_head, 0, part_size );
         memset( mp_end, 0, size - part_size );
      }
   }
   else
   {
      mp_end = dec( mp_end, size );

      // zeroing in case if context was not save => will not be restored in other case modification is not allowed
      // (should be after updating mp_end)
      if( std::nullopt == m_state )
         memset( mp_end, 0, size );
   }

   if( mp_end == mp_tail )
      mp_end = mp_head;

   m_size -= size;
}

bool CircularBuffer::get( void* const p_buffer, const size_t size, const size_t offset ) const
{
   if( nullptr == p_buffer )
   {
      SYS_ERR( "destination buffer pointer is nullptr" );
      return false;
   }
   if( 0 == m_size )
   {
      SYS_WRN( "buffer is empty" );
      return false;
   }
   if( size + offset > m_size )
   {
      SYS_WRN( "buffer size %zu less then requested size %zu + offset %zu", m_size, size, offset );
      return false;
   }

   const size_t part_size_1 = static_cast< size_t >( diff( mp_tail, mp_begin ) );
   const void* p_start = mp_begin;
   if( offset > part_size_1 )
      p_start = inc( mp_head, offset - part_size_1 );
   else if( 0 < offset )
      p_start = inc( p_start, offset );

   const size_t part_size_2 = static_cast< size_t >( diff( mp_tail, p_start ) );
   if( size > part_size_2 )
   {
      memcpy( p_buffer, p_start, part_size_2 );
      memcpy( inc( p_buffer, part_size_2 ), mp_head, size - part_size_2 );
   }
   else
   {
      memcpy( p_buffer, p_start, size );
   }

   return true;
}

bool CircularBuffer::front( void* const p_buffer, const size_t size ) const
{
   return get( p_buffer, size, 0 );
}

bool CircularBuffer::front( RawBuffer& buffer ) const
{
   return front( buffer.ptr, buffer.size );
}

bool CircularBuffer::front( CircularBuffer& buffer ) const
{
   return ePush::Error != buffer.push_back( *this );
}

bool CircularBuffer::move_front( void* const p_buffer, const size_t size )
{
   if( false == front( p_buffer, size ) )
      return false;

   pop_front( size );

   return true;
}

bool CircularBuffer::move_front( RawBuffer& buffer )
{
   return move_front( buffer.ptr, buffer.size );
}

bool CircularBuffer::move_front( CircularBuffer& buffer )
{
   if( false == front( buffer ) )
      return false;

   pop_front( m_size );

   return true;   
}

bool CircularBuffer::cmp( void* const p_buffer, const size_t size, const size_t offset ) const
{
   if( offset > m_size )
      return false;
   if( offset + size > m_size )
      return false;

   size_t part = static_cast< size_t >( diff( mp_tail, mp_begin ) );
   if( offset >= part )
   {
      return 0 == memcmp( inc( mp_head, offset - part ), p_buffer, size );
   }

   if( offset + size > part )
   {
      size_t cmp_part = part - offset;
      if( 0 != memcmp( inc( mp_begin, offset ), p_buffer, cmp_part ) )
         return false;

      return 0 == memcmp( mp_head, inc( p_buffer, cmp_part ), size - cmp_part );
   }

   return 0 == memcmp( inc( mp_begin, offset ), p_buffer, size );
}

bool CircularBuffer::cmp( const RawBuffer& buffer, const size_t offset ) const
{
   return cmp( buffer.ptr, buffer.size, offset );
}

ssize_t CircularBuffer::find( void* const p_buffer, const size_t size, const size_t offset ) const
{
   if( offset > m_size )
      return -1;
   if( offset + size > m_size )
      return -1;

   // size_t part = static_cast< size_t >( diff( mp_tail, mp_begin ) );
   // if( offset >=part )

   return 0;
}

ssize_t CircularBuffer::find( const RawBuffer& buffer, const size_t offset ) const
{
   return find( buffer.ptr, buffer.size, offset );
}

bool CircularBuffer::state_save( )
{
   if( true == m_is_state_locked )
   {
      SYS_ERR( "state locked" );
      return false;
   }
   if( std::nullopt != m_state )
   {
      SYS_ERR( "state already saved" );
      return false;
   }

   m_state = { mp_begin, mp_end, m_size };
   return true;
}

bool CircularBuffer::state_restore( )
{
   if( true == m_is_state_locked )
   {
      SYS_ERR( "state locked" );
      return false;
   }
   if( std::nullopt == m_state )
   {
      SYS_ERR( "state was not saved" );
      return false;
   }

   mp_begin = m_state.value( ).mp_begin;
   mp_end = m_state.value( ).mp_end;
   m_size = m_state.value( ).m_size;
   m_state = std::nullopt;
   return true;
}
