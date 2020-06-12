#include "api/sys/common/CircularBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "C_BUFFER"



using namespace base;



void* const inc( void* const ptr, const size_t value )
{
   return static_cast< void* const >( static_cast< uint8_t* const >( ptr ) + value );
}

ptrdiff_t diff( const void* const ptr1, const void* const ptr2 )
{
   return static_cast< const uint8_t* const >( ptr1 ) - static_cast< const uint8_t* const >( ptr2 );
}



CircularBuffer::CircularBuffer( const size_t _capacity )
   : m_capacity( _capacity )
{
   mp_head = mp_begin = mp_end = malloc( m_capacity + 1 );
   mp_tail = inc( mp_head, m_capacity );
   SYS_INF( "head / tail / begin / end = %p / %p / %p / %p", mp_head, mp_tail, mp_begin, mp_end );
   SYS_INF( "capacity = %zu", m_capacity );
}

CircularBuffer::~CircularBuffer( )
{
   free( mp_head );
}

void CircularBuffer::dump( const eDump type ) const
{
   SYS_INF( "head / tail / begin / end = %p / %p / %p / %p", mp_head, mp_tail, mp_begin, mp_end );
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

bool CircularBuffer::insert_data( void* const p_buffer, const size_t size )
{
   if( size > m_capacity )
   {
      SYS_ERR( "buffer capacity(%zu) not enough", m_capacity );
      return false;
   }
   if( size > free_space( ) )
   {
      SYS_WRN( "buffer is full => overriding data" );
   }

   const size_t part_size = static_cast< size_t >( diff( mp_tail, mp_end ) );
   if( size > part_size )
   {
      SYS_WRN( "----- 1" );
      memcpy( mp_end, p_buffer, part_size );
      memcpy( mp_head, inc( p_buffer, part_size ), size - part_size );
      mp_end = inc( mp_head, size - part_size );
   }
   else
   {
      SYS_WRN( "----- 2" );
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

   return true;
}

bool CircularBuffer::move_data( void* const p_buffer, const size_t size )
{
   if( false == copy_data( p_buffer, size ) )
      return false;

   const size_t part_size = static_cast< size_t >( diff( mp_tail, mp_begin ) );
   if( size > part_size )
   {
      mp_begin = inc( mp_head, size - part_size );
   }
   else
   {
      mp_begin = inc( mp_begin, size );
   }

   if( mp_begin == mp_tail )
      mp_begin = mp_head;

   m_size -= size;

   return true;
}

bool CircularBuffer::copy_data( void* const p_buffer, const size_t size ) const
{
   if( 0 == m_size )
   {
      SYS_WRN( "buffer is empty" );
      return false;
   }
   if( size > m_size )
   {
      SYS_WRN( "buffer size %zu less then requested size %zu", m_size, size );
      return false;
   }

   const size_t part_size = static_cast< size_t >( diff( mp_tail, mp_begin ) );
   if( size > part_size )
   {
      memcpy( p_buffer, mp_begin, part_size );
      memcpy( inc( p_buffer, part_size ), mp_head, size - part_size );
   }
   else
   {
      memcpy( p_buffer, mp_begin, size );
   }

   return true;
}
