#include "ByteBuffer.hpp"

#include "Trace.hpp"
#define CLASS_ABBR "B_BUFFER"



namespace base {



ByteBuffer::ByteBuffer( )
{
}

ByteBuffer::ByteBuffer( const size_t capacity )
{
   if( true == allocate( capacity ) )
      memset( mp_buffer, 0, m_size );
}

ByteBuffer::ByteBuffer( const uint8_t* p_buffer, const size_t size )
{
   if( true == allocate( size ) )
      memcpy( mp_buffer, p_buffer, m_size );
}

ByteBuffer::~ByteBuffer( )
{
   reset( );
}

void ByteBuffer::print( ) const
{
   for( size_t i = 0; i < m_size; ++i )
      printf( "%#x ", mp_buffer[i] );
   printf( "\n" );
}

void ByteBuffer::info( ) const
{
   SYS_INF( "address: %p / capacity: %d / size: %d", mp_buffer, m_capacity, m_size );
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

bool ByteBuffer::push_back( const void* p_buffer, const size_t size, const bool is_reallocate )
{
   // Nothing to push_back
   if( 0 == size ) return false;
   // Buffer is not allocated
   if( nullptr == p_buffer ) return false;
   // Buffer is not allocated
   if( nullptr == mp_buffer ) return false;
   // Reallocating buffer is requested with saving prevous content
   if( ( m_capacity - m_size ) < size && false == is_reallocate ) return false;

   if( ( m_capacity - m_size ) < size )
   {
      if( false == reallocate( m_size + size, true ) )
         return false;
   }

   memcpy( mp_buffer + m_size, p_buffer, size );
   m_size += size;

   return true;
}

bool ByteBuffer::push_back( const uint8_t* p_buffer, const size_t size, const bool is_reallocate )
{
   return push_back( static_cast< const void* >( p_buffer ), size, is_reallocate );
}

bool ByteBuffer::push_back( const char* p_buffer, const size_t size, const bool is_reallocate )
{
   return push_back( static_cast< const void* >( p_buffer ), size, is_reallocate );
}

bool ByteBuffer::push_back( const std::string& string, const bool is_reallocate )
{
   return push_back( string.c_str( ), string.size( ) + 1 /* for termination '\0' */ );
}

bool ByteBuffer::pop_back( const void* p_buffer, const size_t size )
{
   // Not enough data to pop_back
   if( size > m_size ) return false;
   // Nothing to pop_back
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

bool ByteBuffer::pop_back( const uint8_t* p_buffer, const size_t size )
{
   return pop_back( static_cast< const void* >( p_buffer ), size );
}

bool ByteBuffer::pop_back( const char* p_buffer, const size_t size /* here size should also include termination '\0' */ )
{
   return pop_back( static_cast< const void* >( p_buffer ), size );
}

bool ByteBuffer::pop_back( std::string& string, const size_t size /* here size should also include termination '\0' */ )
{
   char p_buffer[ size ];
   if( false == pop_back( static_cast< void* >( p_buffer ), size ) )
      return false;

   if( 0 != p_buffer[ size - 1 ] )
   {
      SYS_WRN( "missing termitation '\0'" );
      p_buffer[ size - 1 ] = 0;
   }

   string = p_buffer;
   return true;
}




} // namespace base
