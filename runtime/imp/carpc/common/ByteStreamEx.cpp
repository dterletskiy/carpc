#include <cstdarg>

#include "carpc/common/ByteStreamEx.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "STREAM"



using namespace carpc;



ByteStreamEx::ByteStreamEx( const std::size_t capacity, const bool is_reallocate_allowed )
   : m_buffer( capacity, false, is_reallocate_allowed )
   // this configuration means that m_buffer will be a linear extendable buffer
{
   SYS_INF( );
}

ByteStreamEx::ByteStreamEx( const void* const buffer, const std::size_t size, const std::size_t capacity, const bool is_reallocate_allowed )
   : m_buffer( ( capacity > size ? capacity : size ), false, is_reallocate_allowed )
   // this configuration means that m_buffer will be a linear extendable buffer
{
   SYS_INF( );
   push_buffer( buffer, size );
}

ByteStreamEx::ByteStreamEx( const ByteStreamEx& other )
   : m_buffer( other.m_buffer )
{
   SYS_INF( );
}

ByteStreamEx::ByteStreamEx( ByteStreamEx&& other )
   : m_buffer( std::move( other.m_buffer ) )
{
   SYS_INF( );
}

ByteStreamEx::~ByteStreamEx( )
{
   SYS_INF( );
}

void ByteStreamEx::debug_message( const char* const message ) const
{
   #ifdef DEBUG_STREAM
      MSG_INF( "%s", message );
   #endif
}

void ByteStreamEx::debug_message_ex( const char* const format, ... ) const
{
   #ifdef DEBUG_STREAM
      va_list argptr;
      va_start( argptr, format );
      printf( format, argptr );
      printf( "\n" );
      va_end( argptr );
   #endif
}


/*****************************************
 *
 * Push methods
 *
 ****************************************/
bool ByteStreamEx::push_buffer( const void* const buffer, const std::size_t size )
{
   #ifdef DEBUG_STREAM
      SYS_DBG( "const void* const buffer: %p / %zu", buffer, size );
   #endif

   return CircularBuffer::ePush::Error != m_buffer.push_back( buffer, size );
}

bool ByteStreamEx::push_buffer( void* const buffer, const std::size_t size )
{
   #ifdef DEBUG_STREAM
      SYS_DBG( "void* const buffer: %p / %zu", buffer, size );
   #endif

   return push_buffer( const_cast< const void* const >( buffer ), size );
}

bool ByteStreamEx::__push__( const RawBuffer& buffer )
{
   debug_message( "RawBuffer" );
   if( false == push( buffer.size ) )
      return false;

   return push_buffer( buffer.ptr, buffer.size );
}

bool ByteStreamEx::__push__( const CircularBuffer& buffer )
{
   debug_message( "CircularBuffer" );
   if( false == push( buffer.size( ) ) )
      return false;

   return CircularBuffer::ePush::Error != m_buffer.push_back( buffer );
}

bool ByteStreamEx::__push__( const ByteStreamEx& stream )
{
   debug_message( "ByteStreamEx" );
   return push( stream.m_buffer );
}

bool ByteStreamEx::__push__( const void* const pointer )
{
   debug_message( "const pointer" );
   return push_buffer( (const void* const)&pointer, sizeof( void* ) );
}

bool ByteStreamEx::__push__( void* const pointer )
{
   debug_message( "pointer" );
   return push_buffer( (void* const)&pointer, sizeof( void* ) );
}

bool ByteStreamEx::__push__( const bool value )
{
   debug_message( "bool" );
   return push( static_cast< tBool >( value ? 1 : 0 ) );
}



/*****************************************
 *
 * Pop methods
 *
 ****************************************/
bool ByteStreamEx::pop_buffer( void* const buffer, const std::size_t size )
{
   #ifdef DEBUG_STREAM
      SYS_DBG( "void* const buffer: %p / %zu", buffer, size );
   #endif

   if( false == m_buffer.front( buffer, size ) )
      return false;

   m_buffer.pop_front( size );
   return true;
}

bool ByteStreamEx::pop_buffer( const void* buffer, const std::size_t size )
{
   #ifdef DEBUG_STREAM
      SYS_DBG( "const void* buffer: %p / %zu", buffer, size );
   #endif

   return pop_buffer( const_cast< void* const >( buffer ), size );
}

bool ByteStreamEx::__pop__( RawBuffer& buffer )
{
   debug_message( "RawBuffer" );
   if( nullptr != buffer.ptr )
      return false;

   if( false == pop( buffer.size ) )
      return false;

   buffer.alloc( buffer.size );
   return pop_buffer( buffer.ptr, buffer.size );
}

bool ByteStreamEx::__pop__( CircularBuffer& buffer )
{
   debug_message( "CircularBuffer" );
   RawBuffer rb;
   if( false == pop( rb ) )
      return false;

   bool result = CircularBuffer::ePush::Error != buffer.push_back( rb );
   rb.free( );

   return result;
}

bool ByteStreamEx::__pop__( ByteStreamEx& stream )
{
   debug_message( "ByteStreamEx" );
   return pop( stream.m_buffer );
}

bool ByteStreamEx::__pop__( const void*& pointer )
{
   debug_message( "const pointer" );
   return pop_buffer( &pointer, sizeof( void* ) );
}

bool ByteStreamEx::__pop__( void*& pointer )
{
   debug_message( "pointer" );
   return pop_buffer( &pointer, sizeof( void* ) );
}

bool ByteStreamEx::__pop__( bool& value )
{
   debug_message( "bool" );
   tBool bool_value = false;
   if( false == pop( bool_value ) )
      return false;

   value = 0 != bool_value;
   return true;
}
