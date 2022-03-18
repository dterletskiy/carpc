#pragma once

#include "api/sys/common/ByteBuffer.hpp"



namespace carpc {



   #define __INTEGRAL_TYPE__( TYPE )         std::is_integral_v< TYPE >
   #define __FLOATING_POINT_TYPE__( TYPE )   std::is_floating_point_v< TYPE >
   #define __ENUM_TYPE__( TYPE )             std::is_enum_v< TYPE >
   #define __REST_TYPES__( TYPE )            !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE > && !std::is_enum_v< TYPE >



   class ByteBufferT
      : public ByteBuffer
   {
         class Transaction
         {
               friend class ByteBuffer;
               friend class ByteBufferT;

            public:
               enum class eType : size_t { push, pop, undefined };

               Transaction( size_t& );
               Transaction( ByteBufferT& );

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
         ByteBufferT( const size_t capacity = 1024 );
         ByteBufferT( const void*, const size_t );
         template< typename TYPE >
            ByteBufferT( const TYPE& );
         ByteBufferT( const ByteBufferT& );
         ~ByteBufferT( );

         /*****************************************
          *
          * Push buffer methods
          *
          ****************************************/
      public:
         bool push( void*, const size_t, const bool is_reallocate = true );
         bool push( const void*, const size_t, const bool is_reallocate = true );
         bool push( const void* const, const bool is_reallocate = true );
         bool push( const std::string&, const bool is_reallocate = true );
         bool push( const ByteBufferT&, const bool is_reallocate = true );
         template< typename TYPE >
            bool push( const std::optional< TYPE >&, const bool is_reallocate = true );
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
         // This method is for integral types and types with floating poing
         template< typename TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
               push( const TYPE&, const bool is_reallocate = true );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
               push( const TYPE&, const bool is_reallocate = true );
         // This method is for user defined types. It calles "to_buffer" method of this type, so it should be implemented in it.
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               push( const TYPE&, const bool is_reallocate = true );
         // This method for multipl push
         template< typename ... TYPES >
            bool push( const TYPES& ... values );

      private:
         bool push_buffer_with_size( const void*, const size_t, const bool is_reallocate );
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
         bool pop( void*, const size_t );
         bool pop( const void*, const size_t );
         bool pop( const void*& );
         bool pop( std::string& );
         bool pop( ByteBufferT& );
         template< typename TYPE >
            bool pop( std::optional< TYPE >& );
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
         // This method is for integral types and types with floating poing
         template< typename TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
               pop( TYPE& );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
               pop( TYPE& );
         // This method is for user defined types. It calles "from_buffer" method of this type, so it should be implemented in it.
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               pop( TYPE& );
         // This method for multipl push
         template< typename ... TYPES >
            bool pop( TYPES& ... values );

      private:
         template< typename TYPE_CONTAINER >
            bool pop_stl_container( TYPE_CONTAINER& );
         template< typename TYPE_CONTAINER >
            bool pop_stl_associative_container( TYPE_CONTAINER& );

         /*****************************************
          *
          * Test buffer methods
          *
          ****************************************/
      public:
         template< typename TYPE >
            bool test( const TYPE& value, const size_t offset = 0 )
            {
               if( offset >= m_size )
                  return false;

               size_t size_backup = m_size;
               m_size -= offset;

               TYPE buffer_value;
               if( false == pop( buffer_value ) )
               {
                  m_size = size_backup;
                  return false;
               }

               m_size = size_backup;
               return value == buffer_value;
            }

         /*****************************************
          *
          * Get buffer methods
          *
          ****************************************/
      public:
         template< typename TYPE >
            bool get( TYPE& value, const size_t offset = 0 )
            {
               if( offset >= m_size )
                  return false;

               size_t size_backup = m_size;
               m_size -= offset;

               bool result = pop( value );

               m_size = size_backup;
               return result;
            }
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
   bool ByteBufferT::push( const std::optional< TYPE >& optional, const bool is_reallocate )
   {
      if( false == m_transaction.start( Transaction::eType::push ) )
         return false;

      if( std::nullopt != optional )
      {
         if( false == push( optional.value( ) ) )
            return m_transaction.error( );
         if( false == push( static_cast< size_t >( 1 ) ) )
            return m_transaction.error( );
      }
      else
      {
         if( false == push( static_cast< size_t >( 0 ) ) )
            return m_transaction.error( );
      }

      return m_transaction.finish( );
   }

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
   typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
   ByteBufferT::push( const TYPE& value, const bool is_reallocate )
   {
      return push( static_cast< const void* >( &value ), sizeof( TYPE ), is_reallocate );
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteBufferT::push( const TYPE& value, const bool is_reallocate )
   {
      if( false == m_transaction.start( Transaction::eType::push ) )
         return false;

      if( false == value.to_buffer( *this ) )
         return m_transaction.error( );
      return m_transaction.finish( );
   }

   template< typename ... TYPES >
   bool ByteBufferT::push( const TYPES& ... values )
   {
      if( false == m_transaction.start( Transaction::eType::push ) )
         return false;

      bool result = true;
      (void)std::initializer_list< int >{ ( result &= push( values ), 0 )... };
      if( false == result )
         return m_transaction.error( );

      return m_transaction.finish( );
   }

   template< typename TYPE >
   typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
   ByteBufferT::push( const TYPE& value, const bool is_reallocate )
   {
      using ENUM_TYPE = std::underlying_type_t< TYPE >;
      return push( static_cast< ENUM_TYPE >( value ), is_reallocate );
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
   bool ByteBufferT::pop( std::optional< TYPE >& optional )
   {
      if( false == m_transaction.start( Transaction::eType::pop ) )
         return false;

      size_t has_value = false;
      if( false == pop( has_value ) )
         return m_transaction.error( );

      if( 1 == has_value )
      {
         TYPE value;
         if( false == pop( value ) )
            return m_transaction.error( );
         optional = value;
      }
      else optional = std::nullopt;

      return m_transaction.finish( );
   }

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
   typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
   ByteBufferT::pop( TYPE& value )
   {
      return pop( static_cast< const void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteBufferT::pop( TYPE& value )
   {
      if( false == m_transaction.start( Transaction::eType::pop ) )
         return false;
      if( false == value.from_buffer( *this ) )
         return m_transaction.error( );
      return m_transaction.finish( );
   }

   template< typename ... TYPES >
   bool ByteBufferT::pop( TYPES& ... values )
   {
      if( false == m_transaction.start( Transaction::eType::pop ) )
         return false;

      bool result = true;
      (void)std::initializer_list< int >{ ( result &= pop( values ), 0 )... };
      if( false == result )
         return m_transaction.error( );

      return m_transaction.finish( );
   }

   template< typename TYPE >
   typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
   ByteBufferT::pop( TYPE& value )
   {
      if( false == m_transaction.start( Transaction::eType::pop ) )
         return false;

      using ENUM_TYPE = std::underlying_type_t< TYPE >;
      ENUM_TYPE _value;
      if( false == pop( _value ) )
         return m_transaction.error( );
      value = static_cast< TYPE >( _value );

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



} // namespace carpc
