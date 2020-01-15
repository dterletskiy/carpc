#pragma once

#include "api/sys/common/Types.hpp"



namespace base {



class ByteBuffer
{
public:
   template< typename TYPE >
      ByteBuffer& operator << ( const TYPE& );
   template< typename TYPE >
      ByteBuffer& operator >> ( TYPE& );

public:
   ByteBuffer( );
   ByteBuffer( const void*, const size_t );
   template< typename TYPE >
      ByteBuffer( const TYPE& );
   ~ByteBuffer( );

public:
   bool allocate( const size_t );
   bool reallocate( const size_t, const bool is_store = false );
   bool trancate( );
   void reset( );

public:
   bool push( const void*, const size_t, const bool is_reallocate = true );
   bool push( const std::string&, const bool is_reallocate = true );
   template< typename TYPE >
      bool push( const std::vector< TYPE >&, const bool is_reallocate = true );
   template< typename TYPE >
      bool push( const std::list< TYPE >&, const bool is_reallocate = true );
   template< typename TYPE_FIRST, typename TYPE_SECOND >
      bool push( const std::pair< TYPE_FIRST, TYPE_SECOND >&, const bool is_reallocate = true );
   template< typename TYPE >
      bool push( const std::set< TYPE >&, const bool is_reallocate = true );
   template< typename TYPE_KEY, typename TYPE_VALUE >
      bool push( const std::map< TYPE_KEY, TYPE_VALUE >&, const bool is_reallocate = true );
   template< typename TYPE >
      typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
         push( const TYPE&, const bool is_reallocate = true );
   template< typename TYPE >
      typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
         push( const TYPE&, const bool is_reallocate = true );

private:
   template< typename TYPE_CONTAINER >
      bool push_stl_container( const TYPE_CONTAINER&, const bool is_reallocate );
   template< typename TYPE_CONTAINER >
      bool push_stl_associative_container( const TYPE_CONTAINER&, const bool is_reallocate );

public:
   bool pop( const void*, const size_t );
   bool pop( std::string& );
   template< typename TYPE >
      bool pop( std::vector< TYPE >& );
   template< typename TYPE >
      bool pop( std::list< TYPE >& );
   template< typename TYPE_FIRST, typename TYPE_SECOND >
      bool pop( std::pair< TYPE_FIRST, TYPE_SECOND >& );
   template< typename TYPE >
      bool pop( std::set< TYPE >& );
   template< typename TYPE_KEY, typename TYPE_VALUE >
      bool pop( std::map< TYPE_KEY, TYPE_VALUE >& );
   template< typename TYPE >
      typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
         pop( TYPE& );
   template< typename TYPE >
      typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
         pop( TYPE& );

private:
   template< typename TYPE_CONTAINER >
      bool pop_stl_container( TYPE_CONTAINER& );
   template< typename TYPE_CONTAINER >
      bool pop_stl_associative_container( TYPE_CONTAINER& );

public:
   const uint8_t* const buffer( ) const;
private:
   uint8_t* mp_buffer = nullptr;

public:
   const size_t capacity( ) const;
private:
   size_t   m_capacity = 0;

public:
   const size_t size( ) const;
private:
   size_t   m_size = 0;

public:
   void dump( ) const;
   void info( ) const;
};



template< typename TYPE >
ByteBuffer::ByteBuffer( const TYPE& data )
{
   if( false == allocate( sizeof( data ) ) ) return;
   *this << data;
}

inline
const uint8_t* const ByteBuffer::buffer( ) const
{
   return mp_buffer;
}

inline
const size_t ByteBuffer::size( ) const
{
   return m_size;
}

inline
const size_t ByteBuffer::capacity( ) const
{
   return m_capacity;
}

template< typename TYPE >
ByteBuffer& ByteBuffer::operator << ( const TYPE& value )
{
   push( value, true );
   return *this;
}

template< typename TYPE >
ByteBuffer& ByteBuffer::operator >> ( TYPE& value )
{
   pop( value );
   return *this;
}

template< typename TYPE >
bool ByteBuffer::push( const std::vector< TYPE >& vector, const bool is_reallocate )
{
   return push_stl_container( vector, is_reallocate );
}

template< typename TYPE >
bool ByteBuffer::push( const std::list< TYPE >& list, const bool is_reallocate )
{
   return push_stl_container( list, is_reallocate );
}

template< typename TYPE_FIRST, typename TYPE_SECOND >
bool ByteBuffer::push( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair, const bool is_reallocate )
{
   if( false == push( pair.first, is_reallocate ) )
      return false;
   if( false == push( pair.second, is_reallocate ) )
      return false;

   return true;
}

template< typename TYPE >
bool ByteBuffer::push( const std::set< TYPE >& set, const bool is_reallocate )
{
   return push_stl_associative_container( set, is_reallocate );
}

template< typename TYPE_KEY, typename TYPE_VALUE >
bool ByteBuffer::push( const std::map< TYPE_KEY, TYPE_VALUE >& map, const bool is_reallocate )
{
   return push_stl_associative_container( map, is_reallocate );
}

template< typename TYPE >
typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
ByteBuffer::push( const TYPE& value, const bool is_reallocate )
{
   return push( static_cast< const void* >( &value ), sizeof( TYPE ), is_reallocate );
}

template< typename TYPE >
typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
ByteBuffer::push( const TYPE& value, const bool is_reallocate )
{
   return value.to_buffer( *this );
}

template< typename TYPE_CONTAINER >
bool ByteBuffer::push_stl_container( const TYPE_CONTAINER& container, const bool is_reallocate )
{
   using TYPE_ITERATOR = typename TYPE_CONTAINER::const_reverse_iterator;
   for( TYPE_ITERATOR iterator = container.crbegin( ); iterator != container.crend( ); ++iterator )
      if( false == push( *iterator, is_reallocate ) )
         return false;

   return push( container.size( ) );
}

template< typename TYPE_CONTAINER >
bool ByteBuffer::push_stl_associative_container( const TYPE_CONTAINER& container, const bool is_reallocate )
{
   return push_stl_container( container, is_reallocate );
}

template< typename TYPE >
bool ByteBuffer::pop( std::vector< TYPE >& vector )
{
   return pop_stl_container( vector );
}

template< typename TYPE >
bool ByteBuffer::pop( std::list< TYPE >& list )
{
   return pop_stl_container( list );
}

template< typename TYPE_FIRST, typename TYPE_SECOND >
bool ByteBuffer::pop( std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
{
   if( false == pop( pair.second ) )
      return false;
   if( false == pop( pair.first ) )
      return false;

   return true;
}

template< typename TYPE >
bool ByteBuffer::pop( std::set< TYPE >& set )
{
   return pop_stl_associative_container( set );
}

template< typename TYPE_KEY, typename TYPE_VALUE >
bool ByteBuffer::pop( std::map< TYPE_KEY, TYPE_VALUE >& map )
{
   return pop_stl_associative_container( map );
}

template< typename TYPE >
typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
ByteBuffer::pop( TYPE& value )
{
   return pop( static_cast< const void* >( &value ), sizeof( TYPE ) );
}

template< typename TYPE >
typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
ByteBuffer::pop( TYPE& value )
{
   return value.from_buffer( *this );
}

template< typename TYPE_CONTAINER >
bool ByteBuffer::pop_stl_container( TYPE_CONTAINER& container )
{
   size_t size = 0;
   // Reading size of string content
   if( false == pop( size ) )
      return false;

   for( size_t index = 0; index < size; ++index )
   {
      typename TYPE_CONTAINER::value_type value;
      if( false == pop( value ) )
         return false;
      container.emplace_back( value );
   }

   return true;
}

template< typename TYPE_CONTAINER >
bool ByteBuffer::pop_stl_associative_container( TYPE_CONTAINER& container )
{
   size_t size = 0;
   // Reading size of string content
   if( false == pop( size ) )
      return false;

   for( size_t index = 0; index < size; ++index )
   {
      // Here value is a pair in case of map and simple type in case of set
      typename TYPE_CONTAINER::value_type value;
      if( false == pop( value ) )
         return false;

      container.emplace( value );
   }

   return true;
}



} // namespace base
