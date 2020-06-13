#include "api/sys/common/ByteStream.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "S_BUFFER"



using namespace base;



ByteStream::ByteStream( const size_t capacity, const bool is_reallocate_allowed )
   : m_buffer( capacity, false, is_reallocate_allowed )
{
}

ByteStream::~ByteStream( )
{
}





bool ByteStream::push( const void* const buffer, const size_t size )
{
   return CircularBuffer::ePush::Error != m_buffer.push_back( buffer, size );
}

bool ByteStream::push( void* const buffer, const size_t size )
{
   return push( const_cast< const void* const >( buffer ), size );
}

bool ByteStream::push( const void* const pointer )
{
   return push( (const void* const)&pointer, sizeof( void* ) );
}

bool ByteStream::push( void* const pointer )
{
   return push( (void* const)&pointer, sizeof( void* ) );
}

bool ByteStream::push( const std::string& string )
{
   const void* p_buffer = static_cast< const void* >( string.c_str( ) );
   const size_t size = string.size( );

   if( false == push( size ) )
      return false;

   return push( p_buffer, size );
}




bool ByteStream::pop( const void* buffer, const size_t size )
{
   return m_buffer.move_front( buffer, size );
}

bool ByteStream::pop( void* buffer, const size_t size )
{
   return pop( const_cast< const void* >( buffer ), size );
}

bool ByteStream::pop( const void*& pointer )
{
   return pop( &pointer, sizeof( void* ) );
}

bool ByteStream::pop( void*& pointer )
{
   return pop( &pointer, sizeof( void* ) );
}

bool ByteStream::pop( std::string& string )
{
   size_t size = 0;
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

