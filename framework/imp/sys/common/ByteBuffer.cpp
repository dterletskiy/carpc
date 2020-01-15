#include "api/sys/common/ByteBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "B_BUFFER"



namespace base {



ByteBuffer::ByteBuffer( )
{
   if( true == allocate( 100 ) )
      memset( mp_buffer, 0, m_size );
}

ByteBuffer::ByteBuffer( const void* p_buffer, const size_t size )
{
   if( true == allocate( size ) )
   {
      // memcpy( mp_buffer, p_buffer, m_size );
      if( false == push( p_buffer, size ) )
      {
         SYS_ERR( "unable to fullfil buffer" );
      }
   }
   else
   {
      SYS_ERR( "unable to allocate %zu bytes", size );
   }
}

ByteBuffer::~ByteBuffer( )
{
   reset( );
}

void ByteBuffer::dump( ) const
{
   for( size_t i = 0; i < m_size; ++i )
      printf( "%#x ", mp_buffer[i] );
   printf( "\n" );
}

void ByteBuffer::info( ) const
{
   SYS_INF( "address: %p / capacity: %zu / size: %zu", mp_buffer, m_capacity, m_size );
}

bool ByteBuffer::allocate( const size_t capacity )
{
   // Nothing to allocate
   if( 0 == capacity ) return false;

   // Buffer already allocated
   if( nullptr != mp_buffer ) return false;

   mp_buffer = (uint8_t*)malloc( capacity );

   // Allocate error
   if( nullptr == mp_buffer ) return false;

   m_capacity = capacity;

   return true;
}

bool ByteBuffer::reallocate( const size_t capacity, const bool is_store )
{
   // Nothing to allocate
   if( 0 == capacity ) return false;

   // What is the sense of reallocation?
   if( capacity == m_capacity ) return false;

   // Allocating buffer because is was not allocated before
   if( nullptr == mp_buffer )
      return allocate( capacity );

   // Storing previous data during reallocation is requested but
   // current data size is more then requested capacity
   if( true == is_store && m_size > capacity )
      return false;

   uint8_t* p_buffer = (uint8_t*)malloc( capacity );

   // Allocate error
   if( nullptr == p_buffer ) return false;

   // Copying current data to new buffer => size will be the same
   if( true == is_store )
      memcpy( p_buffer, mp_buffer, m_size );
   // Storing data was not requested => size should be 0
   else
      m_size = 0;

   free( mp_buffer );
   mp_buffer = p_buffer;
   m_capacity = capacity;

   return true;
}

bool ByteBuffer::trancate( )
{
   if( m_size == m_capacity ) return true;
   return reallocate( m_size, true );
}

void ByteBuffer::reset( )
{
   if( nullptr != mp_buffer )
      free( mp_buffer );

   // Reset data
   mp_buffer = nullptr;
   m_size = m_capacity = 0;   
}

bool ByteBuffer::push( const void* p_buffer, const size_t size, const bool is_reallocate )
{
   // Nothing to push
   if( 0 == size ) return false;
   // Buffer is not allocated
   if( nullptr == p_buffer ) return false;
   // Buffer is not allocated
   if( nullptr == mp_buffer ) return false;
   // Reallocating buffer is requested with saving prevous content
   if( ( m_capacity - m_size ) < size && false == is_reallocate )
      return false;

   if( ( m_capacity - m_size ) < size )
      if( false == reallocate( m_size + size, true ) )
         return false;

   memcpy( mp_buffer + m_size, p_buffer, size );
   m_size += size;

   return true;
}

bool ByteBuffer::push( const std::string& string, const bool is_reallocate )
{
   const char* p_buffer = string.c_str( );
   const size_t size = string.size( );

   // Backup buffer state.
   size_t size_backup = m_size;

   // It should be possible to store next amount of bytes: content size + size of content.
   if( ( m_capacity - m_size ) < ( size + sizeof( size ) ) && false == is_reallocate )
      return false;

   // Storing string content
   if( false == push( static_cast< const void* >( p_buffer ), size, is_reallocate ) )
      return false;

   // Storing size of string. In case of error prevoius buffer state will be restored.
   // In this case stored string content will be deleted.
   if( false == push( size, is_reallocate ) )
   {
      m_size = size_backup;
      return false;
   }

   return true;
}

bool ByteBuffer::pop( const void* p_buffer, const size_t size )
{
   // Not enough data to pop
   if( size > m_size ) return false;
   // Nothing to pop
   if( 0 == size ) return false;
   // Buffer is not allocated
   if( nullptr == p_buffer ) return false;
   // Buffer is not allocated
   if( nullptr == mp_buffer ) return false;

   void* _p_buffer = const_cast< void* >( p_buffer );
   memcpy( _p_buffer, mp_buffer + m_size - size, size );
   m_size -= size;

   return true;
}

bool ByteBuffer::pop( std::string& string )
{
   // Backup buffer state.
   size_t size_backup = m_size;

   size_t size = 0;

   // Reading size of string content
   if( false == pop( size ) )
      return false;

   // Error in case of rest of data in buffer less then size to be read.
   // push previously poped data to restore previous buffer state.
   if( size > m_size )
   {
      // push( size );
      m_size = size_backup;
      return false;
   }

   char p_buffer[ size + 1 ]; // +1 for termitating '\0'
   if( false == pop( static_cast< void* >( p_buffer ), size ) )
      return false;

   // Adding termitating '\0'
   p_buffer[ size + 1 - 1 ] = 0;

   string = p_buffer;
   return true;
}




} // namespace base
