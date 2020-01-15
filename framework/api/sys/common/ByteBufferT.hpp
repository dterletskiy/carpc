#pragma once

#include "api/sys/common/ByteBuffer.hpp"



namespace base {



class ByteBufferT
   : public ByteBuffer
{
   class Transaction
   {
   public:
      enum class eType : size_t { push, pop, undefined };

      Transaction( size_t& );

      bool start( const eType );
      bool finish( );
      bool error( );

   private:
      eType    m_type = eType::undefined;
      size_t   m_recursive_count = 0;
      size_t   m_size_backup = 0;
      size_t&  m_size;
   } m_transaction;

public:
   ByteBufferT( );
   ByteBufferT( const void*, const size_t );
   template< typename TYPE >
      ByteBufferT( const TYPE& );
   ~ByteBufferT( );

   /*****************************************
    *
    * Push buffer methods
    *
    ****************************************/
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
   // This method is for user defined types. It calles "to_buffer" method of this type, so it should be implemented in it.
   template< typename TYPE >
      typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
         push( const TYPE&, const bool is_reallocate = true );

private:
   template< typename TYPE_CONTAINER >
      bool push_stl_container( const TYPE_CONTAINER&, const bool is_reallocate );
   template< typename TYPE_CONTAINER >
      bool push_stl_associative_container( const TYPE_CONTAINER&, const bool is_reallocate );

   /*****************************************
    *
    * Pop buffer methods
    *
    ****************************************/
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
   // This method is for user defined types. It calles "from_buffer" method of this type, so it should be implemented in it.
   template< typename TYPE >
      typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
         pop( TYPE& );

private:
   template< typename TYPE_CONTAINER >
      bool pop_stl_container( TYPE_CONTAINER& );
   template< typename TYPE_CONTAINER >
      bool pop_stl_associative_container( TYPE_CONTAINER& );
};



template< typename TYPE >
ByteBufferT::ByteBufferT( const TYPE& data )
   : ByteBuffer( data )
   , m_transaction( m_size )
{
}



/*****************************************
 *
 * Push buffer methods
 *
 ****************************************/
template< typename TYPE >
bool ByteBufferT::push( const std::vector< TYPE >& vector, const bool is_reallocate )
{
   return push_stl_container( vector, is_reallocate );
}

template< typename TYPE >
bool ByteBufferT::push( const std::list< TYPE >& list, const bool is_reallocate )
{
   return push_stl_container( list, is_reallocate );
}

template< typename TYPE_FIRST, typename TYPE_SECOND >
bool ByteBufferT::push( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair, const bool is_reallocate )
{
   if( false == m_transaction.start( Transaction::eType::push ) )
      return false;

   if( false == push( pair.first, is_reallocate ) )
      return m_transaction.error( );
   if( false == push( pair.second, is_reallocate ) )
      return m_transaction.error( );

   return m_transaction.finish( );
}

template< typename TYPE >
bool ByteBufferT::push( const std::set< TYPE >& set, const bool is_reallocate )
{
   return push_stl_associative_container( set, is_reallocate );
}

template< typename TYPE_KEY, typename TYPE_VALUE >
bool ByteBufferT::push( const std::map< TYPE_KEY, TYPE_VALUE >& map, const bool is_reallocate )
{
   return push_stl_associative_container( map, is_reallocate );
}

template< typename TYPE >
typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
ByteBufferT::push( const TYPE& value, const bool is_reallocate )
{
   return push( static_cast< const void* >( &value ), sizeof( TYPE ), is_reallocate );
}

template< typename TYPE >
typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
ByteBufferT::push( const TYPE& value, const bool is_reallocate )
{
   if( false == m_transaction.start( Transaction::eType::push ) )
      return false;

   if( false == value.to_buffer( *this ) )
      m_transaction.error( );
   return m_transaction.finish( );
}

template< typename TYPE_CONTAINER >
bool ByteBufferT::push_stl_container( const TYPE_CONTAINER& container, const bool is_reallocate )
{
   if( false == m_transaction.start( Transaction::eType::push ) )
      return false;

   using TYPE_ITERATOR = typename TYPE_CONTAINER::const_reverse_iterator;
   for( TYPE_ITERATOR iterator = container.crbegin( ); iterator != container.crend( ); ++iterator )
      if( false == push( *iterator, is_reallocate ) )
         return m_transaction.error( );

   if( false == push( container.size( ) ) )
      return m_transaction.error( );

   return m_transaction.finish( );
}

template< typename TYPE_CONTAINER >
bool ByteBufferT::push_stl_associative_container( const TYPE_CONTAINER& container, const bool is_reallocate )
{
   return push_stl_container( container, is_reallocate );
}



/*****************************************
 *
 * Pop buffer methods
 *
 ****************************************/

template< typename TYPE >
bool ByteBufferT::pop( std::vector< TYPE >& vector )
{
   return pop_stl_container( vector );
}

template< typename TYPE >
bool ByteBufferT::pop( std::list< TYPE >& list )
{
   return pop_stl_container( list );
}

template< typename TYPE_FIRST, typename TYPE_SECOND >
bool ByteBufferT::pop( std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
{
   if( false == m_transaction.start( Transaction::eType::pop ) )
      return false;

   if( false == pop( pair.second ) )
      return m_transaction.error( );
   if( false == pop( pair.first ) )
      return m_transaction.error( );

   return m_transaction.finish( );
}

template< typename TYPE >
bool ByteBufferT::pop( std::set< TYPE >& set )
{
   return pop_stl_associative_container( set );
}

template< typename TYPE_KEY, typename TYPE_VALUE >
bool ByteBufferT::pop( std::map< TYPE_KEY, TYPE_VALUE >& map )
{
   return pop_stl_associative_container( map );
}

template< typename TYPE >
typename std::enable_if_t< std::is_integral_v< TYPE > || std::is_floating_point_v< TYPE >, bool >
ByteBufferT::pop( TYPE& value )
{
   return pop( static_cast< const void* >( &value ), sizeof( TYPE ) );
}

template< typename TYPE >
typename std::enable_if_t< !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE >, bool >
ByteBufferT::pop( TYPE& value )
{
   if( false == m_transaction.start( Transaction::eType::pop ) )
      return false;
   if( false == value.from_buffer( *this ) )
      m_transaction.error( );
   return m_transaction.finish( );
}

template< typename TYPE_CONTAINER >
bool ByteBufferT::pop_stl_container( TYPE_CONTAINER& container )
{
   if( false == m_transaction.start( Transaction::eType::pop ) )
      return false;

   size_t size = 0;
   // Reading size of string content
   if( false == pop( size ) )
      return m_transaction.error( );

   for( size_t index = 0; index < size; ++index )
   {
      typename TYPE_CONTAINER::value_type value;
      if( false == pop( value ) )
         return m_transaction.error( );
      container.emplace_back( value );
   }

   return m_transaction.finish( );
}

template< typename TYPE_CONTAINER >
bool ByteBufferT::pop_stl_associative_container( TYPE_CONTAINER& container )
{
   if( false == m_transaction.start( Transaction::eType::pop ) )
      return false;

   size_t size = 0;
   // Reading size of string content
   if( false == pop( size ) )
      return m_transaction.error( );

   for( size_t index = 0; index < size; ++index )
   {
      // Here value is a pair in case of map and simple type in case of set
      typename TYPE_CONTAINER::value_type value;
      if( false == pop( value ) )
         return m_transaction.error( );

      container.emplace( value );
   }

   return m_transaction.finish( );
}



} // namespace base
