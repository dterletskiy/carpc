#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/common/CircularBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "C_BUFFER"



using namespace carpc;



const double s_reallocation_coefficient = 1.5;



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

CircularBuffer::CircularBuffer(
         const std::size_t capacity,
         const bool is_overlap_allowed,
         const bool is_reallocate_allowed,
         const bool auto_free
      )
   : m_capacity( capacity )
   , m_is_overlap_allowed( is_overlap_allowed )
   , m_is_reallocate_allowed( is_reallocate_allowed )
   , m_auto_free( auto_free )
{
   SYS_INF( );

   mp_head = malloc( m_capacity + 1 );
   mp_tail = inc( mp_head, m_capacity );
   mp_begin = mp_end = mp_head;
}

CircularBuffer::CircularBuffer( const CircularBuffer& other )
   : m_capacity( other.m_capacity )
   , m_is_overlap_allowed( other.m_is_overlap_allowed )
   , m_is_reallocate_allowed( other.m_is_reallocate_allowed )
   , m_auto_free( other.m_auto_free )
{
   SYS_INF( );

   mp_head = malloc( m_capacity + 1 );
   mp_tail = inc( mp_head, m_capacity );
   mp_begin = mp_end = mp_head;

   other.front( mp_head, other.size( ) );
   mp_end = inc( mp_begin, other.size( ) );

   m_size = other.m_size;
}

CircularBuffer::CircularBuffer( CircularBuffer&& other )
   : m_capacity( other.m_capacity )
   , m_size( other.m_size )
   , m_is_overlap_allowed( other.m_is_overlap_allowed )
   , m_is_reallocate_allowed( other.m_is_reallocate_allowed )
   , m_auto_free( other.m_auto_free )
{
   SYS_INF( );

   mp_head = other.mp_head;
   mp_tail = other.mp_tail;
   mp_begin = other.mp_begin;
   mp_end = other.mp_end;

   other.mp_head = nullptr;
   other.mp_tail = nullptr;
   other.mp_begin = nullptr;
   other.mp_end = nullptr;
   other.m_capacity = 0;
   other.m_size = 0;
   other.m_auto_free = false;
}

CircularBuffer::~CircularBuffer( )
{
   SYS_INF( );

   if( true == m_auto_free )
      free( mp_head );
}

bool CircularBuffer::reallocate( std::size_t capacity )
{
   if( 0 == capacity )
      capacity = s_reallocation_coefficient * m_capacity;

   void* p_head = malloc( capacity + 1 );
   if( nullptr == p_head )
      return false;

   std::size_t size = std::min( capacity, m_size );
   front( p_head, size );
   free( mp_head );
   m_capacity = capacity;
   m_size = size;
   mp_head = p_head;
   mp_tail = inc( mp_head, m_capacity );
   mp_begin = mp_head;
   mp_end = inc( mp_begin, m_size );
   // if( mp_tail == mp_end ) // @TDA: perhaps not needed
   //    mp_end = mp_head;

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
   for( std::size_t i = 0; i < m_capacity; ++i )
      printf( "%#x ", static_cast< uint8_t* >( mp_head )[i] );

   printf( "\n" );
}

void CircularBuffer::dump_logic( ) const
{
   printf( "Logic dump: " );
   const std::size_t part_size = static_cast< std::size_t >( diff( mp_tail, mp_begin ) );
   if( m_size > part_size )
   {
      for( std::size_t i = 0; i < part_size; ++i )
         printf( "%#x ", static_cast< uint8_t* >( mp_begin )[i] );
      for( std::size_t i = 0; i < m_size - part_size; ++i )
         printf( "%#x ", static_cast< uint8_t* >( mp_head )[i] );
   }
   else
   {
      for( std::size_t i = 0; i < m_size; ++i )
         printf( "%#x ", static_cast< uint8_t* >( mp_begin )[i] );
   }

   printf( "\n" );
}

CircularBuffer::ePush CircularBuffer::push_back( const void* const p_buffer, const std::size_t size, const std::optional< bool > is_reallocate )
{
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

      if( false == reallocate( s_reallocation_coefficient * ( m_size + size ) ) )
         return ePush::Error;
      result = ePush::Realloc;
   }

   const std::size_t part_size = static_cast< std::size_t >( diff( mp_tail, mp_end ) );
   if( size > part_size )
   {
      std::memcpy( mp_end, p_buffer, part_size );
      std::memcpy( mp_head, inc( p_buffer, part_size ), size - part_size );
      mp_end = inc( mp_head, size - part_size );
   }
   else
   {
      std::memcpy( mp_end, p_buffer, size );
      mp_end = inc( mp_end, size );
   }

   // if( mp_end == mp_tail )
   //    mp_end = mp_head; // @TDA: perhaps not needed

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
   const std::size_t part_size = static_cast< std::size_t >( diff( buffer.mp_tail, buffer.mp_begin ) );
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

void CircularBuffer::pop_front( const std::size_t size )
{
   if( 0 == m_size )
      return;

   if( size >= m_size )
      return reset( );

   const std::size_t part_size = static_cast< std::size_t >( diff( mp_tail, mp_begin ) );
   if( size > part_size )
   {
      // zeroing pop data bytes
      std::memset( mp_begin, 0, part_size );
      std::memset( mp_head, 0, size - part_size );

      mp_begin = inc( mp_head, size - part_size );
   }
   else
   {
      // zeroing pop data bytes
      std::memset( mp_begin, 0, size );

      mp_begin = inc( mp_begin, size );
   }

   if( mp_begin == mp_tail )
      mp_begin = mp_head;

   m_size -= size;
}

void CircularBuffer::pop_back( const std::size_t size )
{
   if( 0 == m_size )
      return;

   if( size >= m_size )
      return reset( );

   const std::size_t part_size = static_cast< std::size_t >( diff( mp_end, mp_head ) );
   if( size > part_size )
   {
      mp_end = dec( mp_tail, size - part_size );

      // zeroing pop data bytes
      std::memset( mp_head, 0, part_size );
      std::memset( mp_end, 0, size - part_size );
   }
   else
   {
      mp_end = dec( mp_end, size );

      // zeroing pop data bytes
      std::memset( mp_end, 0, size );
   }

   // if( mp_end == mp_tail )
   //    mp_end = mp_head; // @TDA: perhaps not needed

   m_size -= size;
}

bool CircularBuffer::get( void* const p_buffer, const std::size_t size, const std::size_t offset ) const
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

   const std::size_t part_size_1 = static_cast< std::size_t >( diff( mp_tail, mp_begin ) );
   const void* p_start = mp_begin;
   if( offset > part_size_1 )
      p_start = inc( mp_head, offset - part_size_1 );
   else if( 0 < offset )
      p_start = inc( p_start, offset );

   const std::size_t part_size_2 = static_cast< std::size_t >( diff( mp_tail, p_start ) );
   if( size > part_size_2 )
   {
      std::memcpy( p_buffer, p_start, part_size_2 );
      std::memcpy( inc( p_buffer, part_size_2 ), mp_head, size - part_size_2 );
   }
   else
   {
      std::memcpy( p_buffer, p_start, size );
   }

   return true;
}

bool CircularBuffer::front( void* const p_buffer, const std::size_t size ) const
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

bool CircularBuffer::move_front( void* const p_buffer, const std::size_t size )
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

bool CircularBuffer::cmp( void* const p_buffer, const std::size_t size, const std::size_t offset ) const
{
   if( offset > m_size )
      return false;
   if( offset + size > m_size )
      return false;

   std::size_t part = static_cast< std::size_t >( diff( mp_tail, mp_begin ) );
   if( offset >= part )
   {
      return 0 == memcmp( inc( mp_head, offset - part ), p_buffer, size );
   }

   if( offset + size > part )
   {
      std::size_t cmp_part = part - offset;
      if( 0 != memcmp( inc( mp_begin, offset ), p_buffer, cmp_part ) )
         return false;

      return 0 == memcmp( mp_head, inc( p_buffer, cmp_part ), size - cmp_part );
   }

   return 0 == memcmp( inc( mp_begin, offset ), p_buffer, size );
}

bool CircularBuffer::cmp( const RawBuffer& buffer, const std::size_t offset ) const
{
   return cmp( buffer.ptr, buffer.size, offset );
}

ssize_t CircularBuffer::find( void* const p_buffer, const std::size_t size, const std::size_t offset ) const
{
   if( offset > m_size )
      return -1;
   if( offset + size > m_size )
      return -1;

   // std::size_t part = static_cast< std::size_t >( diff( mp_tail, mp_begin ) );
   // if( offset >=part )

   return 0;
}

ssize_t CircularBuffer::find( const RawBuffer& buffer, const std::size_t offset ) const
{
   return find( buffer.ptr, buffer.size, offset );
}
