#include "carpc/common/ByteStream.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "STREAM"



using namespace carpc;



ByteStream::ByteStream( const std::size_t capacity, const bool is_reallocate_allowed )
   : m_buffer( capacity, false, is_reallocate_allowed )
   // this configuration means that m_buffer will be a linear extendable buffer
{
   SYS_INF( );
}

ByteStream::ByteStream( const void* const buffer, const std::size_t size, const std::size_t capacity, const bool is_reallocate_allowed )
   : m_buffer( ( capacity > size ? capacity : size ), false, is_reallocate_allowed )
   // this configuration means that m_buffer will be a linear extendable buffer
{
   SYS_INF( );
   push( buffer, size );
}

ByteStream::ByteStream( const ByteStream& other )
   : m_buffer( other.m_buffer )
{
   SYS_INF( );
}

ByteStream::ByteStream( ByteStream&& other )
   : m_buffer( std::move( other.m_buffer ) )
{
   SYS_INF( );
}

ByteStream::~ByteStream( )
{
   SYS_INF( );
}

void ByteStream::debug_message( const char* const message ) const
{
   #ifdef DEBUG_STREAM
      MSG_INF( "%s", message );
   #endif
}


/*****************************************
 *
 * Push methods
 *
 ****************************************/
bool ByteStream::push( const void* const buffer, const std::size_t size )
{
   debug_message( "const buffer" );
   return CircularBuffer::ePush::Error != m_buffer.push_back( buffer, size );
}

bool ByteStream::push( void* const buffer, const std::size_t size )
{
   debug_message( "buffer" );
   return push( const_cast< const void* const >( buffer ), size );
}

bool ByteStream::push( const RawBuffer& buffer )
{
   debug_message( "RawBuffer" );
   if( false == push( buffer.size ) )
      return false;

   return push( buffer.ptr, buffer.size );
}

bool ByteStream::push( const CircularBuffer& buffer )
{
   debug_message( "CircularBuffer" );
   if( false == push( buffer.size( ) ) )
      return false;

   return CircularBuffer::ePush::Error != m_buffer.push_back( buffer );
}

bool ByteStream::push( const ByteStream& stream )
{
   debug_message( "ByteStream" );
   return push( stream.m_buffer );
}

bool ByteStream::push( const void* const pointer )
{
   debug_message( "const pointer" );
   return push( (const void* const)&pointer, sizeof( void* ) );
}

bool ByteStream::push( void* const pointer )
{
   debug_message( "pointer" );
   return push( (void* const)&pointer, sizeof( void* ) );
}

bool ByteStream::push( const bool value )
{
   debug_message( "bool" );
   return push( static_cast< tBool >( value ? 1 : 0 ) );
}

bool ByteStream::push( const std::string& string )
{
   debug_message( "std::string" );
   const void* p_buffer = static_cast< const void* >( string.c_str( ) );
   const std::size_t size = string.size( );

   if( false == push( size ) )
      return false;

   return push( p_buffer, size );
}



/*****************************************
 *
 * Pop methods
 *
 ****************************************/
bool ByteStream::pop( void* const buffer, const std::size_t size )
{
   debug_message( "buffer" );
   if( false == m_buffer.front( buffer, size ) )
      return false;

   m_buffer.pop_front( size );
   return true;

   // return m_buffer.move_front( buffer, size );
}

bool ByteStream::pop( const void* buffer, const std::size_t size )
{
   debug_message( "const buffer" );
   return pop( const_cast< void* const >( buffer ), size );
}

bool ByteStream::pop( RawBuffer& buffer )
{
   debug_message( "RawBuffer" );
   if( nullptr != buffer.ptr )
      return false;

   if( false == pop( buffer.size ) )
      return false;

   buffer.alloc( buffer.size );
   return pop( buffer.ptr, buffer.size );
}

bool ByteStream::pop( CircularBuffer& buffer )
{
   debug_message( "CircularBuffer" );
   RawBuffer rb;
   if( false == pop( rb ) )
      return false;

   bool result = CircularBuffer::ePush::Error != buffer.push_back( rb );
   rb.free( );

   return result;
}

bool ByteStream::pop( ByteStream& stream )
{
   debug_message( "ByteStream" );
   return pop( stream.m_buffer );
}

bool ByteStream::pop( const void*& pointer )
{
   debug_message( "const pointer" );
   return pop( &pointer, sizeof( void* ) );
}

bool ByteStream::pop( void*& pointer )
{
   debug_message( "pointer" );
   return pop( &pointer, sizeof( void* ) );
}

bool ByteStream::pop( bool& value )
{
   debug_message( "bool" );
   tBool bool_value = false;
   if( false == pop( bool_value ) )
      return false;

   value = 0 != bool_value;
   return true;
}

bool ByteStream::pop( std::string& string )
{
   debug_message( "std::string" );
   std::size_t size = 0;
   if( false == pop( size ) )
      return false;

   char p_string[ size + 1 ]; // +1 for termitating '\0'

   if( false == pop( (void*)p_string, size ) )
      return false;

   // Adding termitating '\0'
   p_string[ size + 1 - 1 ] = 0;

   string = p_string;
   return true;
}
