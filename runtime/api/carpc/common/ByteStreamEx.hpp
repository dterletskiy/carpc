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
#include "carpc/common/TypeInfo.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "STREAM"



namespace carpc {

   using tBool = char;

   #define __REST_TYPES__( TYPE )            !CARPC_IS_INTEGRAL_TYPE( TYPE ) \
                                          && !CARPC_IS_FLOATING_POINT_TYPE( TYPE ) \
                                          && !CARPC_IS_ENUM_TYPE( TYPE ) \
                                          && !CARPC_IS_GPB_TYPE( TYPE )
}



namespace carpc {



   class ByteStreamEx
   {
      public:
         template< typename TYPE >
            ByteStreamEx& operator << ( const TYPE& value );
         template< typename TYPE >
            ByteStreamEx& operator >> ( TYPE& value );

         template< typename ... TYPES >
            ByteStreamEx& operator << ( const TYPES& ... value );
         template< typename ... TYPES >
            ByteStreamEx& operator >> ( TYPES& ... value );

      public:
         // Serializing all arguments and result will be stored in raw_buffer.
         // In this case RawBuffer will point to allocated memory and this memory MUST be freed by user.
         template< typename ... TYPES >
         static bool serialize( RawBuffer& buffer, const TYPES& ... args )
         {
            ByteStreamEx stream( 1024, true );
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
            ByteStreamEx stream( buffer.size, true );
            stream.push( buffer.ptr, buffer.size );
            return stream.pop( args... );
         }

      public:
         ByteStreamEx( const std::size_t capacity = 4096, const bool is_reallocate_allowed = true );
         ByteStreamEx( const void* const buffer, const std::size_t size, const std::size_t capacity = 0, const bool is_reallocate_allowed = true );
         ByteStreamEx( const ByteStreamEx& other );
         ByteStreamEx( ByteStreamEx&& other );
         ~ByteStreamEx( );

      /*****************************************
       *
       * Push methods
       *
       ****************************************/
      public:
         // This method for single push
         template< typename TYPE >
            bool push( const TYPE& value );
         // This method for multiple push
         template< typename ... TYPES >
            bool push( const TYPES& ... values );

      private:
         bool __push__( const RawBuffer& buffer );
         bool __push__( const CircularBuffer& buffer );
         bool __push__( const ByteStreamEx& stream );
         bool __push__( const void* const pointer );
         bool __push__( void* const pointer );
         bool __push__( const bool value );
         template < typename TYPE, std::size_t N >
            bool __push__( const TYPE (&array)[ N ] );
         template< typename TYPE >
            bool __push__( const std::optional< TYPE >& value );
         template< typename TYPE >
            bool __push__( const std::unique_ptr< TYPE >& value );
         template< typename TYPE >
            bool __push__( const std::shared_ptr< TYPE >& value );
         // This method is for integral types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
               __push__( const TYPE& value );
         // This method is for floating point types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
               __push__( const TYPE& value );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
               __push__( const TYPE& value );
         // This method is for GPB types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
               __push__( const TYPE& value );
         // Rest types
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               __push__( const TYPE& value );
         // std containers
         template< typename TYPE >
            bool __push__( const std::basic_string< TYPE >& string );
         template< typename TYPE >
            bool __push__( const std::vector< TYPE >& vector );
         template< typename TYPE >
            bool __push__( const std::list< TYPE >& list );
         template< typename TYPE >
            bool __push__( const std::queue< TYPE >& queue );
         template< typename TYPE >
            bool __push__( const std::deque< TYPE >& deque );
         template< typename TYPE_FIRST, typename TYPE_SECOND >
            bool __push__( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair );
         template< typename TYPE >
            bool __push__( const std::set< TYPE >& set );
         template< typename TYPE_KEY, typename TYPE_VALUE >
            bool __push__( const std::map< TYPE_KEY, TYPE_VALUE >& map );

      private:
         template< typename TYPE_CONTAINER >
            bool push_stl_container( const TYPE_CONTAINER& container );
         template< typename TYPE_CONTAINER >
            bool push_stl_associative_container( const TYPE_CONTAINER& container );

      private:
         template< typename TYPE >
            bool __push_metadata__( );

      private:
         bool push_buffer( const void* const buffer, const std::size_t size );
         bool push_buffer( void* const buffer, const std::size_t size );

      /*****************************************
       *
       * Pop methods
       *
       ****************************************/
      public:
         // This method for multiple pop
         template< typename ... TYPES >
            bool pop( TYPES& ... values );
         // This method for single pop
         template< typename TYPE >
            bool pop( TYPE& value );

      private:
         bool __pop__( RawBuffer& buffer );
         bool __pop__( CircularBuffer& buffer );
         bool __pop__( ByteStreamEx& stream );
         bool __pop__( const void*& pointer );
         bool __pop__( void*& pointer );
         bool __pop__( bool& value );
         template < typename TYPE, std::size_t N >
            bool __pop__( TYPE (&array)[ N ] );
         template< typename TYPE >
            bool __pop__( std::optional< TYPE >& value );
         template< typename TYPE >
            bool __pop__( std::unique_ptr< TYPE >& value );
         template< typename TYPE >
            bool __pop__( std::shared_ptr< TYPE >& value );
         // This method is for integral types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
               __pop__( TYPE& value );
         // This method is for floating point types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
               __pop__( TYPE& value );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
               __pop__( TYPE& value );
         // This method is for GPB types
         template< typename TYPE >
            typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
               __pop__( TYPE& value );
         // Rest types
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               __pop__( TYPE& value );
         // std containers
         template< typename TYPE >
            bool __pop__( std::basic_string< TYPE >& string );
         template< typename TYPE >
            bool __pop__( std::vector< TYPE >& vector );
         template< typename TYPE >
            bool __pop__( std::list< TYPE >& list );
         template< typename TYPE >
            bool __pop__( std::queue< TYPE >& queue );
         template< typename TYPE >
            bool __pop__( std::deque< TYPE >& deque );
         template< typename TYPE_FIRST, typename TYPE_SECOND >
            bool __pop__( std::pair< TYPE_FIRST, TYPE_SECOND >& pair );
         template< typename TYPE >
            bool __pop__( std::set< TYPE >& set );
         template< typename TYPE_KEY, typename TYPE_VALUE >
            bool __pop__( std::map< TYPE_KEY, TYPE_VALUE >& map );

      private:
         template< typename TYPE_CONTAINER >
            bool pop_stl_container( TYPE_CONTAINER& container );
         template< typename TYPE_CONTAINER >
            bool pop_stl_associative_container( TYPE_CONTAINER& container );

      private:
         template< typename TYPE >
            bool __pop_metadata__( );

      private:
         bool pop_buffer( void* const buffer, const std::size_t size );
         bool pop_buffer( const void* buffer, const std::size_t size );

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
         void debug_message_ex( const char* const format, ... ) const;
      private:
         CircularBuffer m_buffer;
   };

}



namespace carpc {

   /*****************************************
    *
    * Operators
    *
    ****************************************/
   template< typename TYPE >
   ByteStreamEx& ByteStreamEx::operator << ( const TYPE& value )
   {
      push( value );
      return *this;
   }

   template< typename TYPE >
   ByteStreamEx& ByteStreamEx::operator >> ( TYPE& value )
   {
      pop( value );
      return *this;
   }

   template< typename ... TYPES >
   ByteStreamEx& ByteStreamEx::operator << ( const TYPES& ... value )
   {
      push( value... );
      return *this;
   }

   template< typename ... TYPES >
   ByteStreamEx& ByteStreamEx::operator >> ( TYPES& ... value )
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
   bool ByteStreamEx::push( const TYPES& ... values )
   {
      bool result = true;
      (void)std::initializer_list< int >{ ( result &= push( values ), 0 )... };

      return result;
   }

   template< typename TYPE >
   bool ByteStreamEx::push( const TYPE& value )
   {
      if( false == __push_metadata__< TYPE >( ) )
         return false;

      return __push__( value );
   }

   template< typename TYPE >
   bool ByteStreamEx::__push_metadata__( )
   {
      using CONSTLESS_TYPE = typename std::remove_const< TYPE >::type;

      static auto id = types::TTypeInfo< CONSTLESS_TYPE >::id;
      return push_buffer( static_cast< const void* >( &id ), sizeof( id ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
   ByteStreamEx::__push__( const TYPE& value )
   {
      debug_message( "integral" );
      return push_buffer( static_cast< const void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
   ByteStreamEx::__push__( const TYPE& value )
   {
      debug_message( "floating" );
      return push_buffer( static_cast< const void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
   ByteStreamEx::__push__( const TYPE& value )
   {
      debug_message( "enum" );
      using ENUM_TYPE = std::underlying_type_t< TYPE >;
      return push( static_cast< ENUM_TYPE >( value ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
   ByteStreamEx::__push__( const TYPE& value )
   {
      debug_message( "gpb" );
      std::string ss;
      value.SerializeToString( &ss );
      return push( ss );
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteStreamEx::__push__( const TYPE& value )
   {
      debug_message( "other" );
      return value.to_stream( *this );
   }

   template < typename TYPE, std::size_t N >
   bool ByteStreamEx::__push__( const TYPE (&array)[ N ] )
   {
      debug_message( "array" );
      bool result = true;
      for ( std::size_t index = 0; index < N; ++index )
         result &= push( array[ index ] );

      return result;
   }

   template< typename TYPE >
   bool ByteStreamEx::__push__( const std::optional< TYPE >& optional )
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
   bool ByteStreamEx::__push__( const std::unique_ptr< TYPE >& ptr )
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
   bool ByteStreamEx::__push__( const std::shared_ptr< TYPE >& ptr )
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
   bool ByteStreamEx::__push__( const std::basic_string< TYPE >& string )
   {
      debug_message( "std::basic_string" );
      const std::size_t size = string.size( );

      if( false == push( size ) )
         return false;

      const void* p_buffer = static_cast< const void* >( string.c_str( ) );
      return push_buffer( p_buffer, size * sizeof( TYPE ) );
   }

   template< typename TYPE >
   bool ByteStreamEx::__push__( const std::vector< TYPE >& vector )
   {
      debug_message( "std::vector" );
      return push_stl_container( vector );
   }

   template< typename TYPE >
   bool ByteStreamEx::__push__( const std::list< TYPE >& list )
   {
      debug_message( "std::list" );
      return push_stl_container( list );
   }

   template< typename TYPE >
   bool ByteStreamEx::__push__( const std::queue< TYPE >& queue )
   {
      debug_message( "std::queue" );
      return push_stl_container( queue );
   }

   template< typename TYPE >
   bool ByteStreamEx::__push__( const std::deque< TYPE >& deque )
   {
      debug_message( "std::deque" );
      return push_stl_container( deque );
   }

   template< typename TYPE_FIRST, typename TYPE_SECOND >
   bool ByteStreamEx::__push__( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
   {
      debug_message( "std::pair" );
      if( false == push( pair.first ) )
         return false;
      if( false == push( pair.second ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStreamEx::__push__( const std::set< TYPE >& set )
   {
      debug_message( "std::set" );
      return push_stl_associative_container( set );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   bool ByteStreamEx::__push__( const std::map< TYPE_KEY, TYPE_VALUE >& map )
   {
      debug_message( "std::map" );
      return push_stl_associative_container( map );
   }

   template< typename TYPE_CONTAINER >
   bool ByteStreamEx::push_stl_container( const TYPE_CONTAINER& container )
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
   bool ByteStreamEx::push_stl_associative_container( const TYPE_CONTAINER& container )
   {
      return push_stl_container( container );
   }



   /*****************************************
    *
    * Pop methods
    *
    ****************************************/
   template< typename ... TYPES >
   bool ByteStreamEx::pop( TYPES& ... values )
   {
      bool result = true;
      (void)std::initializer_list< int >{ ( result &= __pop__( values ), 0 )... };

      return result;
   }

   template< typename TYPE >
   bool ByteStreamEx::pop( TYPE& value )
   {
      if( false == __pop_metadata__< TYPE >( ) )
         return false;

      return __pop__( value );
   }

   template< typename TYPE >
   bool ByteStreamEx::__pop_metadata__( )
   {
      using CONSTLESS_TYPE = typename std::remove_const< TYPE >::type;
      types::tID id;

      if( false == pop_buffer( static_cast< void* >( &id ), sizeof( id ) ) )
         return false;

      if( types::TTypeInfo< CONSTLESS_TYPE >::id != id )
      {
         MSG_ERR( "type id mismatch: '%u' != '%u'", types::TTypeInfo< CONSTLESS_TYPE >::id, id );
         return false;
      }

      return true;
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_INTEGRAL_TYPE( TYPE ), bool >
   ByteStreamEx::__pop__( TYPE& value )
   {
      debug_message( "integral" );
      return pop_buffer( const_cast< void* >( static_cast< const void* >( &value ) ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_FLOATING_POINT_TYPE( TYPE ), bool >
   ByteStreamEx::__pop__( TYPE& value )
   {
      debug_message( "floating" );
      return pop_buffer( static_cast< void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< CARPC_IS_ENUM_TYPE( TYPE ), bool >
   ByteStreamEx::__pop__( TYPE& value )
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
   typename std::enable_if_t< CARPC_IS_GPB_TYPE( TYPE ), bool >
   ByteStreamEx::__pop__( TYPE& value )
   {
      debug_message( "gpb" );
      std::string ss;
      if( false == pop( ss ) )
         return false;
      return value.ParseFromString( ss );
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteStreamEx::__pop__( TYPE& value )
   {
      debug_message( "other" );
      return value.from_stream( *this );
   }

   template < typename TYPE, std::size_t N >
   bool ByteStreamEx::__pop__( TYPE (&array)[ N ] )
   {
      debug_message( "array" );
      bool result = true;
      for ( std::size_t index = 0; index < N; ++index )
         result &= pop( array[ index ] );

      return result;
   }

   template< typename TYPE >
   bool ByteStreamEx::__pop__( std::optional< TYPE >& optional )
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
   bool ByteStreamEx::__pop__( std::unique_ptr< TYPE >& ptr )
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
   bool ByteStreamEx::__pop__( std::shared_ptr< TYPE >& ptr )
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
   bool ByteStreamEx::__pop__( std::basic_string< TYPE >& string )
   {
      debug_message( "std::basic_string" );
      std::size_t size = 0;
      if( false == pop( size ) )
         return false;

      TYPE* buffer = (TYPE*)malloc( ( size + 1 ) * sizeof( TYPE ) );

      if( false == pop_buffer( (void*)buffer, size * sizeof( TYPE ) ) )
      {
         free( buffer );
         return false;
      }

      // Adding termitating '\0'
      buffer[ size + 1 - 1 ] = 0;

      string = buffer;

      free( buffer );
      return true;
   }

   template< typename TYPE >
   bool ByteStreamEx::__pop__( std::vector< TYPE >& vector )
   {
      debug_message( "std::vector" );
      return pop_stl_container( vector );
   }

   template< typename TYPE >
   bool ByteStreamEx::__pop__( std::list< TYPE >& list )
   {
      debug_message( "std::list" );
      return pop_stl_container( list );
   }

   template< typename TYPE >
   bool ByteStreamEx::__pop__( std::queue< TYPE >& queue )
   {
      debug_message( "std::queue" );
      return pop_stl_container( queue );
   }

   template< typename TYPE >
   bool ByteStreamEx::__pop__( std::deque< TYPE >& deque )
   {
      debug_message( "std::deque" );
      return pop_stl_container( deque );
   }

   template< typename TYPE_FIRST, typename TYPE_SECOND >
   bool ByteStreamEx::__pop__( std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
   {
      debug_message( "std::pair" );
      if( false == pop( pair.first ) )
         return false;
      if( false == pop( pair.second ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStreamEx::__pop__( std::set< TYPE >& set )
   {
      debug_message( "std::set" );
      return pop_stl_associative_container( set );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   bool ByteStreamEx::__pop__( std::map< TYPE_KEY, TYPE_VALUE >& map )
   {
      debug_message( "std::map" );
      return pop_stl_associative_container( map );
   }

   template< typename TYPE_CONTAINER >
   bool ByteStreamEx::pop_stl_container( TYPE_CONTAINER& container )
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
   bool ByteStreamEx::pop_stl_associative_container( TYPE_CONTAINER& container )
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
   const void* const ByteStreamEx::buffer( ) const
   {
      return m_buffer.ptr( );
   }

   inline
   std::size_t ByteStreamEx::size( ) const
   {
      return m_buffer.size( );
   }

   inline
   std::size_t ByteStreamEx::capacity( ) const
   {
      return m_buffer.capacity( );
   }

   inline
   void ByteStreamEx::dump( ) const
   {
      return m_buffer.dump( );
   }

   inline
   void ByteStreamEx::reset( )
   {
      m_buffer.reset( );
   }
 
   inline
   void ByteStreamEx::buffer( const void*& pointer, std::size_t& size ) const
   {
      pointer = m_buffer.ptr( );
      size = m_buffer.size( );
   }

} // namespace carpc



#undef CLASS_ABBR
