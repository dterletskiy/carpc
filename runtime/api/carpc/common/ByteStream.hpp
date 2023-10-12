#pragma once

#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <optional>
#include <memory>

#include "carpc/helpers/macros/types.hpp"
#include "carpc/common/CircularBuffer.hpp"



namespace carpc {



   #define __REST_TYPES__( TYPE )            !CARPC_IS_INTEGRAL_TYPE( TYPE ) \
                                                && !CARPC_IS_FLOATING_POINT_TYPE( TYPE ) \
                                                && !CARPC_IS_ENUM_TYPE( TYPE ) \
                                                && !CARPC_IS_GPB_TYPE( TYPE )

   using tBool = char;



   class ByteStream
   {
      public:
         template< typename TYPE >
            ByteStream& operator << ( const TYPE& value );
         template< typename TYPE >
            ByteStream& operator >> ( TYPE& value );

         template< typename ... TYPES >
            ByteStream& operator << ( const TYPES& ... value );
         template< typename ... TYPES >
            ByteStream& operator >> ( TYPES& ... value );

      public:
         // Serializing all arguments and result will be stored in raw_buffer.
         // In this case RawBuffer will point to allocated memory and this memory MUST be freed by user.
         template< typename ... TYPES >
         static bool serialize( RawBuffer& buffer, const TYPES& ... args )
         {
            ByteStream stream( 1024, true );
            stream.m_buffer.auto_free( false );
            stream.push( args... );
            // in this case raw buffer always will be linear and we can just return it
            buffer = stream.m_buffer.buffer( );
            return true;
         }
         // Deserializing raw_buffer to arguments.
         // raw_buffer will not be changed afterward.
         template< typename ... TYPES >
         static bool deserialize( const RawBuffer& buffer, TYPES& ... args )
         {
            ByteStream stream( buffer.size, true );
            stream.push( buffer.ptr, buffer.size );
            return stream.pop( args... );
         }

      public:
         ByteStream( const std::size_t capacity = 4096, const bool is_reallocate_allowed = true );
         ByteStream( const void* const buffer, const std::size_t size, const std::size_t capacity = 0, const bool is_reallocate_allowed = true );
         ByteStream( const ByteStream& other );
         ByteStream( ByteStream&& other );
         ~ByteStream( );

      /*****************************************
       *
       * Push methods
       *
       ****************************************/
      public:
         // This method for multiple push
         template< typename ... TYPES >
            bool push( const TYPES& ... values );

      public:
         bool push( const void* const buffer, const std::size_t size );
         bool push( void* const buffer, const std::size_t size );
         bool push( const RawBuffer& buffer );
         bool push( const CircularBuffer& buffer );
         bool push( const ByteStream& stream );
         bool push( const void* const pointer );
         bool push( void* const pointer );
         bool push( const std::string& string );
         bool push( const bool value );
         template < typename TYPE, std::size_t N >
            bool push( const TYPE (&array)[ N ] );
         template< typename TYPE >
            bool push( const std::optional< TYPE >& value );
         template< typename TYPE >
            bool push( const std::unique_ptr< TYPE >& value );
         template< typename TYPE >
            bool push( const std::shared_ptr< TYPE >& value );
         template< typename TYPE >
            bool push( const std::vector< TYPE >& vector );
         template< typename TYPE >
            bool push( const std::list< TYPE >& list );
         template< typename TYPE >
            bool push( const std::queue< TYPE >& queue );
         template< typename TYPE >
            bool push( const std::deque< TYPE >& deque );
         template< typename TYPE_FIRST, typename TYPE_SECOND >
            bool push( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair );
         template< typename TYPE >
            bool push( const std::set< TYPE >& set );
         template< typename TYPE_KEY, typename TYPE_VALUE >
            bool push( const std::map< TYPE_KEY, TYPE_VALUE >& map );
         // This method is for integral types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
               push( const TYPE& value );
         // This method is for floating point types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
               push( const TYPE& value );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
               push( const TYPE& value );
         // This method is for user defined types. It calles "to_stream" method of this type, so it should be implemented in it.
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               push( const TYPE& value );
         // This method is for GPB types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
               push( const TYPE& value );

      private:
         template< typename TYPE_CONTAINER >
            bool push_stl_container( const TYPE_CONTAINER& );
         template< typename TYPE_CONTAINER >
            bool push_stl_associative_container( const TYPE_CONTAINER& );

      /*****************************************
       *
       * Pop methods
       *
       ****************************************/
      public:
         // This method for multiple pop
         template< typename ... TYPES >
            bool pop( TYPES& ... values );

      public:
         bool pop( void* const buffer, const std::size_t size );
         bool pop( const void* buffer, const std::size_t size );
         bool pop( RawBuffer& buffer );
         bool pop( CircularBuffer& buffer );
         bool pop( ByteStream& stream );
         bool pop( const void*& pointer );
         bool pop( void*& pointer );
         bool pop( std::string& string );
         bool pop( bool& value );
         template < typename TYPE, std::size_t N >
            bool pop( TYPE (&array)[ N ] );
         template< typename TYPE >
            bool pop( std::optional< TYPE >& value );
         template< typename TYPE >
            bool pop( std::unique_ptr< TYPE >& value );
         template< typename TYPE >
            bool pop( std::shared_ptr< TYPE >& value );
         template< typename TYPE >
            bool pop( std::vector< TYPE >& vactor );
         template< typename TYPE >
            bool pop( std::list< TYPE >& list );
         template< typename TYPE >
            bool pop( std::queue< TYPE >& queue );
         template< typename TYPE >
            bool pop( std::deque< TYPE >& deque );
         template< typename TYPE_FIRST, typename TYPE_SECOND >
            bool pop( std::pair< TYPE_FIRST, TYPE_SECOND >& pair );
         template< typename TYPE >
            bool pop( std::set< TYPE >& set );
         template< typename TYPE_KEY, typename TYPE_VALUE >
            bool pop( std::map< TYPE_KEY, TYPE_VALUE >& map );
         // This method is for integral types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
               pop( TYPE& value );
         // This method is for floating point types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
               pop( TYPE& value );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
               pop( TYPE& value );
         // This method is for user defined types. It calles "from_stream" method of this type, so it should be implemented in it.
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               pop( TYPE& value );
         // This method is for GPB types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
               pop( TYPE& value );

      private:
         template< typename TYPE_CONTAINER >
            bool pop_stl_container( TYPE_CONTAINER& );
         template< typename TYPE_CONTAINER >
            bool pop_stl_associative_container( TYPE_CONTAINER& );

      /*****************************************
       *
       * Other methods
       *
       ****************************************/
      public:
         void buffer( const void*& pointer, std::size_t& size ) const;
         const void* const buffer( ) const;
         std::size_t size( ) const;
         std::size_t capacity( ) const;
         void dump( ) const;
         void reset( );
      private:
         void debug_message( const char* const message ) const;
      private:
         CircularBuffer m_buffer;
   };



   /*****************************************
    *
    * Operators
    *
    ****************************************/
   template< typename TYPE >
   ByteStream& ByteStream::operator << ( const TYPE& value )
   {
      push( value );
      return *this;
   }

   template< typename TYPE >
   ByteStream& ByteStream::operator >> ( TYPE& value )
   {
      pop( value );
      return *this;
   }

   template< typename ... TYPES >
   ByteStream& ByteStream::operator << ( const TYPES& ... value )
   {
      push( value... );
      return *this;
   }

   template< typename ... TYPES >
   ByteStream& ByteStream::operator >> ( TYPES& ... value )
   {
      pop( value... );
      return *this;
   }



   /*****************************************
    *
    * Push methods
    *
    ****************************************/
   template< typename ... TYPES >
   bool ByteStream::push( const TYPES& ... values )
   {
      bool result = true;
      (void)std::initializer_list< int >{ ( result &= push( values ), 0 )... };

      return result;
   }

   template < typename TYPE, std::size_t N >
   bool ByteStream::push( const TYPE (&array)[ N ] )
   {
      debug_message( "array" );
      bool result = true;
      for ( std::size_t index = 0; index < N; ++index )
         result &= push( array[ index ] );

      return result;
   }

   template< typename TYPE >
   bool ByteStream::push( const std::optional< TYPE >& optional )
   {
      debug_message( "std::optional" );
      if( false == push( std::nullopt != optional ) )
         return false;

      if( std::nullopt != optional )
         if( false == push( optional.value( ) ) )
            return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::push( const std::unique_ptr< TYPE >& ptr )
   {
      debug_message( "std::unique_ptr" );
      if( false == push( nullptr != ptr ) )
         return false;

      if( nullptr != ptr )
         if( false == push( *ptr ) )
            return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::push( const std::shared_ptr< TYPE >& ptr )
   {
      debug_message( "std::shared_ptr" );
      if( false == push( nullptr != ptr ) )
         return false;

      if( nullptr != ptr )
         if( false == push( *ptr ) )
            return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::push( const std::vector< TYPE >& vector )
   {
      debug_message( "std::vector" );
      return push_stl_container( vector );
   }

   template< typename TYPE >
   bool ByteStream::push( const std::list< TYPE >& list )
   {
      debug_message( "std::list" );
      return push_stl_container( list );
   }

   template< typename TYPE >
   bool ByteStream::push( const std::queue< TYPE >& queue )
   {
      debug_message( "std::queue" );
      return push_stl_container( queue );
   }

   template< typename TYPE >
   bool ByteStream::push( const std::deque< TYPE >& deque )
   {
      debug_message( "std::deque" );
      return push_stl_container( deque );
   }

   template< typename TYPE_FIRST, typename TYPE_SECOND >
   bool ByteStream::push( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
   {
      debug_message( "std::pair" );
      if( false == push( pair.first ) )
         return false;
      if( false == push( pair.second ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::push( const std::set< TYPE >& set )
   {
      debug_message( "std::set" );
      return push_stl_associative_container( set );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   bool ByteStream::push( const std::map< TYPE_KEY, TYPE_VALUE >& map )
   {
      debug_message( "std::map" );
      return push_stl_associative_container( map );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      debug_message( "integral" );
      return push( static_cast< const void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      debug_message( "floating" );
      return push( static_cast< const void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      debug_message( "enum" );
      using ENUM_TYPE = std::underlying_type_t< TYPE >;
      return push( static_cast< ENUM_TYPE >( value ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      debug_message( "other" );
      return value.to_stream( *this );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      debug_message( "gpb" );
      std::string ss;
      value.SerializeToString( &ss );
      return push( ss );
   }

   template< typename TYPE_CONTAINER >
   bool ByteStream::push_stl_container( const TYPE_CONTAINER& container )
   {
      if( false == push( container.size( ) ) )
         return false;

      using TYPE_ITERATOR = typename TYPE_CONTAINER::const_iterator;
      for( TYPE_ITERATOR iterator = container.cbegin( ); iterator != container.cend( ); ++iterator )
         if( false == push( *iterator ) )
            return false;

      return true;
   }

   template< typename TYPE_CONTAINER >
   bool ByteStream::push_stl_associative_container( const TYPE_CONTAINER& container )
   {
      return push_stl_container( container );
   }



   /*****************************************
    *
    * Pop methods
    *
    ****************************************/
   template< typename ... TYPES >
   bool ByteStream::pop( TYPES& ... values )
   {
      bool result = true;
      (void)std::initializer_list< int >{ ( result &= pop( values ), 0 )... };

      return result;
   }

   template < typename TYPE, std::size_t N >
   bool ByteStream::pop( TYPE (&array)[ N ] )
   {
      debug_message( "array" );
      bool result = true;
      for ( std::size_t index = 0; index < N; ++index )
         result &= pop( array[ index ] );

      return result;
   }

   template< typename TYPE >
   bool ByteStream::pop( std::optional< TYPE >& optional )
   {
      debug_message( "optional" );
      bool has_value = false;
      if( false == pop( has_value ) )
         return false;

      if( true == has_value )
      {
         TYPE value;
         if( false == pop( value ) )
            return false;
         optional = value;
      }
      else optional = std::nullopt;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::pop( std::unique_ptr< TYPE >& ptr )
   {
      debug_message( "unique_ptr" );
      bool is_data = false;
      if( false == pop( is_data ) )
         return false;

      if( false == is_data )
      {
         ptr.reset( );
         return true;
      }

      if( nullptr == ptr )
         ptr = std::make_unique< TYPE >( );
      if( false == pop( *ptr ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::pop( std::shared_ptr< TYPE >& ptr )
   {
      debug_message( "shared_ptr" );
      bool is_data = false;
      if( false == pop( is_data ) )
         return false;

      if( false == is_data )
      {
         ptr.reset( );
         return true;
      }

      if( nullptr == ptr )
         ptr = std::make_shared< TYPE >( );
      if( false == pop( *ptr ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::pop( std::vector< TYPE >& vector )
   {
      debug_message( "std::vector" );
      return pop_stl_container( vector );
   }

   template< typename TYPE >
   bool ByteStream::pop( std::list< TYPE >& list )
   {
      debug_message( "std::list" );
      return pop_stl_container( list );
   }

   template< typename TYPE >
   bool ByteStream::pop( std::queue< TYPE >& queue )
   {
      debug_message( "std::queue" );
      return pop_stl_container( queue );
   }

   template< typename TYPE >
   bool ByteStream::pop( std::deque< TYPE >& deque )
   {
      debug_message( "std::deque" );
      return pop_stl_container( deque );
   }

   template< typename TYPE_FIRST, typename TYPE_SECOND >
   bool ByteStream::pop( std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
   {
      debug_message( "std::pair" );
      if( false == pop( pair.first ) )
         return false;
      if( false == pop( pair.second ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::pop( std::set< TYPE >& set )
   {
      debug_message( "std::set" );
      return pop_stl_associative_container( set );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   bool ByteStream::pop( std::map< TYPE_KEY, TYPE_VALUE >& map )
   {
      debug_message( "std::map" );
      return pop_stl_associative_container( map );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      debug_message( "integral" );
      return pop( const_cast< void* >( static_cast< const void* >( &value ) ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      debug_message( "floating" );
      return pop( const_cast< void* >( static_cast< const void* >( &value ) ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      debug_message( "enum" );
      using ENUM_TYPE = std::underlying_type_t< TYPE >;
      ENUM_TYPE _value{ };
      if( false == pop( _value ) )
         return false;
      value = static_cast< TYPE >( _value );

      return true;
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      debug_message( "other" );
      return value.from_stream( *this );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      debug_message( "gpb" );
      std::string ss;
      pop( ss );
      return value.ParseFromString( ss );
   }

   template< typename TYPE_CONTAINER >
   bool ByteStream::pop_stl_container( TYPE_CONTAINER& container )
   {
      std::size_t size = 0;
      if( false == pop( size ) )
         return false;

      container = TYPE_CONTAINER{ };
      for( std::size_t index = 0; index < size; ++index )
      {
         typename TYPE_CONTAINER::value_type value;
         if( false == pop( value ) )
            return false;
         container.emplace_back( std::move( value ) );
      }

      return true;
   }

   template< typename TYPE_CONTAINER >
   bool ByteStream::pop_stl_associative_container( TYPE_CONTAINER& container )
   {
      std::size_t size = 0;
      if( false == pop( size ) )
         return false;

      container = TYPE_CONTAINER{ };
      for( std::size_t index = 0; index < size; ++index )
      {
         // Here value is a pair in case of map and simple type in case of set
         typename TYPE_CONTAINER::value_type value;
         if( false == pop( value ) )
            return false;

         container.emplace( std::move( value ) );
      }

      return true;
   }



   /*****************************************
    *
    * Other methods
    *
    ****************************************/
   inline
   const void* const ByteStream::buffer( ) const
   {
      return m_buffer.ptr( );
   }

   inline
   std::size_t ByteStream::size( ) const
   {
      return m_buffer.size( );
   }

   inline
   std::size_t ByteStream::capacity( ) const
   {
      return m_buffer.capacity( );
   }

   inline
   void ByteStream::dump( ) const
   {
      return m_buffer.dump( );
   }

   inline
   void ByteStream::reset( )
   {
      m_buffer.reset( );
   }
 
   inline
   void ByteStream::buffer( const void*& pointer, std::size_t& size ) const
   {
      pointer = m_buffer.ptr( );
      size = m_buffer.size( );
   }

} // namespace carpc
