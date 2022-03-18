#include <cstring>

#include "api/sys/helpers/functions/pointer.hpp"
#include "api/sys/common/StackBuffer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "S_BUFFER"



using namespace carpc;



StackBuffer::StackBuffer( const std::size_t capacity, const bool is_reallocate_allowed )
   : m_is_reallocate_allowed( is_reallocate_allowed )
{
   if( true == allocate( capacity ) )
      fill( );
}

StackBuffer::~StackBuffer( )
{
   if( nullptr != mp_buffer )
      free( mp_buffer );
}

void StackBuffer::dump( ) const
{
   for( std::size_t i = 0; i < m_size; ++i )
      printf( "%#x ", static_cast< uint8_t* >( mp_buffer )[i] );
   printf( "\n" );
}

void StackBuffer::info( ) const
{
   SYS_INF( "address: %p / capacity: %zu / size: %zu", mp_buffer, m_capacity, m_size );
}

void StackBuffer::fill( const char symbol )
{
   std::memset( mp_buffer, symbol, m_capacity );
}

bool StackBuffer::allocate( const std::size_t capacity )
{
   if( 0 == capacity )
      return false;
   if( nullptr != mp_buffer )
      return false;

   mp_buffer = malloc( capacity );
   if( nullptr == mp_buffer )
      return false;

   m_capacity = capacity;

   return true;
}

bool StackBuffer::reallocate( const std::size_t capacity, const bool is_store )
{
   if( 0 == capacity )
      return false;
   if( capacity == m_capacity )
      return false;

   if( nullptr == mp_buffer )
      return allocate( capacity );

   void* p_buffer = malloc( capacity );
   if( nullptr == p_buffer )
      return false;

   // Copying current data to new buffer => size will be the same
   if( true == is_store )
   {
      std::size_t size = std::min( capacity, m_size );
      std::memcpy( p_buffer, mp_buffer, size );
      m_size = size;
   }
   // Storing data was not requested => size should be 0
   else m_size = 0;

   free( mp_buffer );
   mp_buffer = p_buffer;
   m_capacity = capacity;

   return true;
}

bool StackBuffer::push_back( const void* const p_buffer, const std::size_t size, const std::optional< bool > is_reallocate )
{
   if( 0 == size )
      return false;
   if( nullptr == p_buffer )
      return false;
   if( nullptr == mp_buffer )
      return false;

   // Reallocating buffer is requested with saving prevous content
   if( ( m_capacity - m_size ) < size )
   {
      if( true == is_reallocate.value_or( m_is_reallocate_allowed ) )
      {
         if( false == reallocate( 1.5 * ( m_size + size ), true ) )
            return false;
      }
      else return false;
   }

   std::memcpy( inc( mp_buffer, m_size ), p_buffer, size );

   return true;
}


void StackBuffer::pop_back( const std::size_t size )
{
   if( size >= m_size )
      return reset( );

   m_size -= size;
}

bool StackBuffer::back( void* const p_buffer, const std::size_t size )
{
   if( size > m_size )
      return false;
   if( 0 == size )
      return false;
   if( nullptr == p_buffer )
      return false;
   if( nullptr == mp_buffer )
      return false;

   std::memcpy( p_buffer, inc( mp_buffer, m_size - size ), size );

   return true;
}

bool StackBuffer::front( void* const p_buffer, const std::size_t size, const std::size_t offset )
{
   if( size + offset > m_size )
      return false;
   if( 0 == size )
      return false;
   if( nullptr == p_buffer )
      return false;
   if( nullptr == mp_buffer )
      return false;

   std::memcpy( p_buffer, inc( mp_buffer, offset ), size );

   return true;
}