#include "api/sys/common/ByteBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "B_BUFFER"



using namespace base;



ByteBuffer::ByteBuffer( const size_t capacity )
{
   if( true == allocate( capacity ) )
      fill( );
}

ByteBuffer::ByteBuffer( const void* p_buffer, const size_t size )
{
   if( false == allocate( size ) )
   {
      SYS_ERR( "unable to allocate %zu bytes", size );
      return;
   }

   if( false == push( p_buffer, size ) )
   {
      SYS_ERR( "unable to fullfil buffer" );
      return;
   }

   // SYS_INF( "created: " );
   // dump( );
}

ByteBuffer::ByteBuffer( const ByteBuffer& buffer )
{
   if( false == allocate( buffer.m_size ) )
   {
      SYS_ERR( "unable to allocate %zu bytes", buffer.m_size );
      return;
   }

   if( false == push( buffer ) )
   {
      SYS_ERR( "unable to fullfil buffer" );
      return;
   }

   // SYS_INF( "created: " );
   // dump( );
}

ByteBuffer::~ByteBuffer( )
{
   reset( );
}

void ByteBuffer::dump( ) const
{
   // leads to core dump
   // std::stringstream ss;
   // for( size_t i = 0; i < m_size; ++i )
   //    ss << std::setfill('0') << std::setw(2) << std::showbase << std::hex << static_cast< size_t >( mp_buffer[i] ) << " ";
   // SYS_INF( "%s", ss.str( ).c_str( ) );

   for( size_t i = 0; i < m_size; ++i )
      printf( "%#x ", static_cast< uint8_t* >( mp_buffer )[i] );
   printf( "\n" );
}

void ByteBuffer::info( ) const
{
   SYS_INF( "address: %p / capacity: %zu / size: %zu", mp_buffer, m_capacity, m_size );
}

void ByteBuffer::fill( const char symbol )
{
   memset( mp_buffer, symbol, m_capacity );
}

bool ByteBuffer::allocate( const size_t capacity )
{
   // Nothing to allocate
   if( 0 == capacity ) return false;

   // Buffer already allocated
   if( nullptr != mp_buffer ) return false;

   mp_buffer = malloc( capacity );

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

   void* p_buffer = malloc( capacity );

   // Allocate error
   if( nullptr == p_buffer ) return false;

   // Copying current data to new buffer => size will be the same
   if( true == is_store )
      memcpy( p_buffer, mp_buffer, m_size );
   // Storing data was not requested => size should be 0
   else m_size = 0;

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

/*****************************************
 *
 * Read / Write buffer methods
 *
 ****************************************/
bool ByteBuffer::write( const void* p_buffer, const size_t size, const bool is_reallocate )
{
   // Nothing to push
   if( 0 == size ) return false;
   // Buffer is not allocated
   if( nullptr == p_buffer ) return false;
   // Buffer is not allocated
   if( nullptr == mp_buffer ) return false;
   // Reallocating buffer is requested with saving prevous content
   if( ( m_capacity - m_size ) < size )
   {
      if( true == is_reallocate )
      {
         if( false == reallocate( m_size + size, true ) )
            return false;
      }
      else return false;
   }

   memcpy( static_cast< uint8_t* >( mp_buffer ) + m_size, p_buffer, size );

   return true;
}

bool ByteBuffer::read( const void* p_buffer, const size_t size )
{
   // Not enough data to pop
   if( size > m_size ) return false;
   // Nothing to pop
   if( 0 == size ) return false;
   // Buffer is not allocated
   if( nullptr == p_buffer ) return false;
   // Buffer is not allocated
   if( nullptr == mp_buffer ) return false;

   memcpy( const_cast< void* >( p_buffer ), static_cast< uint8_t* >( mp_buffer ) + m_size - size, size );

   return true;
}



/*****************************************
 *
 * Push buffer methods
 *
 ****************************************/
bool ByteBuffer::push( void* p_buffer, const size_t size, const bool is_reallocate )
{
   return push( const_cast< const void* >( p_buffer ), size, is_reallocate );
}

bool ByteBuffer::push( const void* p_buffer, const size_t size, const bool is_reallocate )
{
   if( false == write( p_buffer, size, is_reallocate ) )
      return false;

   m_size += size;
   return true;
}

bool ByteBuffer::push( const void* const p_buffer, const bool is_reallocate )
{
   if( false == write( &p_buffer, sizeof( void* ), is_reallocate ) )
      return false;

   m_size += sizeof( void* );
   return true;
}

bool ByteBuffer::push( const std::string& string, const bool is_reallocate )
{
   const void* p_buffer = static_cast< const void* >( string.c_str( ) );
   const size_t size = string.size( );

   return push_buffer_with_size( p_buffer, size, is_reallocate );
}

bool ByteBuffer::push( const ByteBuffer& _buffer, const bool is_reallocate )
{
   const void* p_buffer = _buffer.mp_buffer;
   const size_t size = _buffer.m_size;

   return push_buffer_with_size( p_buffer, size, is_reallocate );
}

bool ByteBuffer::push_buffer_with_size( const void* p_buffer, const size_t size, const bool is_reallocate )
{
   // Backup buffer state.
   size_t size_backup = m_size;

   // It should be possible to store next amount of bytes: content size + size of content.
   if( ( m_capacity - m_size ) < ( size + sizeof( size ) ) && false == is_reallocate )
      return false;

   // Storing buffer content
   if( false == push( p_buffer, size, is_reallocate ) )
      return false;

   // Storing size of buffer. In case of error prevoius buffer state will be restored.
   // In this case stored buffer content will be deleted.
   if( false == push( size, is_reallocate ) )
   {
      m_size = size_backup;
      return false;
   }

   return true;
}



/*****************************************
 *
 * Pop buffer methods
 *
 ****************************************/
bool ByteBuffer::pop( void* p_buffer, const size_t size )
{
   return pop( const_cast< const void* >( p_buffer ), size );
}

bool ByteBuffer::pop( const void* p_buffer, const size_t size )
{
   if( false == read( p_buffer, size ) )
      return false;

   m_size -= size;
   return true;
}

bool ByteBuffer::pop( const void*& p_buffer )
{
   if( false == read( &p_buffer, sizeof( void* ) ) )
      return false;

   m_size -= sizeof( void* );
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

bool ByteBuffer::pop( ByteBuffer& _buffer )
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
      m_size = size_backup;
      return false;
   }

   void* p_buffer = malloc( size );
   if( false == pop( p_buffer, size ) )
   {
      free( p_buffer );
      return false;
   }

   if( false == _buffer.push( p_buffer, size ) )
   {
      free( p_buffer );
      return false;
   }

   free( p_buffer );
   return true;
}
