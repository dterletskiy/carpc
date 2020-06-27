#pragma once

#include "api/sys/common/Types.hpp"
#include "api/sys/common/CircularBuffer.hpp"


namespace base {



   #define __INTEGRAL_TYPE__( TYPE )         std::is_integral_v< TYPE >
   #define __FLOATING_POINT_TYPE__( TYPE )   std::is_floating_point_v< TYPE >
   #define __ENUM_TYPE__( TYPE )             std::is_enum_v< TYPE >
   #define __REST_TYPES__( TYPE )            !std::is_integral_v< TYPE > && !std::is_floating_point_v< TYPE > && !std::is_enum_v< TYPE >



   class ByteStream
   {
      public:
         template< typename TYPE >
            ByteStream& operator << ( const TYPE& value );
         template< typename TYPE >
            ByteStream& operator >> ( TYPE& value );

      public:
         template< typename ... TYPES >
         static RawBuffer serialize( const TYPES& ... args )
         {
            ByteStream stream( 1024, true );
            stream.m_buffer.auto_free( false );
            stream.push( args... );
            return stream.m_buffer.buffer( );
         }
         template< typename ... TYPES >
         static bool deserialize( const RawBuffer& buffer, TYPES& ... args )
         {
            ByteStream stream( buffer.size, true );
            stream.push( buffer.ptr, buffer.size );
            return stream.pop( args... );
         }

      public:
         ByteStream( const size_t capacity = 4096, const bool is_reallocate_allowed = true );
         ByteStream( const ByteStream& stream );
         ~ByteStream( );

      /*****************************************
       *
       * Push methods
       *
       ****************************************/
      public:
         bool push( const void* const buffer, const size_t size );
         bool push( void* const buffer, const size_t size );
         bool push( const RawBuffer& buffer );
         bool push( const void* const pointer );
         bool push( void* const pointer );
         bool push( const std::string& string );
         bool push( const bool value );
         template< typename TYPE >
            bool push( const std::optional< TYPE >& value );
         template< typename TYPE >
            bool push( const std::vector< TYPE >& vector );
         template< typename TYPE >
            bool push( const std::list< TYPE >& list );
         template< typename TYPE_FIRST, typename TYPE_SECOND >
            bool push( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair );
         template< typename TYPE >
            bool push( const std::set< TYPE >& set );
         template< typename TYPE_KEY, typename TYPE_VALUE >
            bool push( const std::map< TYPE_KEY, TYPE_VALUE >& map );
         // This method is for integral types and types with floating point
         template< typename TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
               push( const TYPE& value );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
               push( const TYPE& value );
         // This method is for user defined types. It calles "to_stream" method of this type, so it should be implemented in it.
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               push( const TYPE& value );
         // This method for multipl push
         template< typename ... TYPES >
            bool push( const TYPES& ... values );

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
         bool pop( void* const buffer, const size_t size );
         bool pop( const void* buffer, const size_t size );
         bool pop( RawBuffer& buffer );
         bool pop( const void*& pointer );
         bool pop( void*& pointer );
         bool pop( std::string& string );
         bool pop( bool& value );
         template< typename TYPE >
            bool pop( std::optional< TYPE >& value );
         template< typename TYPE >
            bool pop( std::vector< TYPE >& vactor );
         template< typename TYPE >
            bool pop( std::list< TYPE >& list );
         template< typename TYPE_FIRST, typename TYPE_SECOND >
            bool pop( std::pair< TYPE_FIRST, TYPE_SECOND >& pair );
         template< typename TYPE >
            bool pop( std::set< TYPE >& set );
         template< typename TYPE_KEY, typename TYPE_VALUE >
            bool pop( std::map< TYPE_KEY, TYPE_VALUE >& map );
         // This method is for integral types and types with floating poing
         template< typename TYPE >
            typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
               pop( TYPE& value );
         // This method is for enumerations.
         template< typename TYPE >
            typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
               pop( TYPE& value );
         // This method is for user defined types. It calles "from_stream" method of this type, so it should be implemented in it.
         template< typename TYPE >
            typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
               pop( TYPE& value );
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
       * Get methods
       *
       ****************************************/
      public:
         // This method for multipl get
         template< typename ... TYPES >
            bool get( TYPES& ... values );

      private:
         template< typename TYPE_CONTAINER >
            bool get_stl_container( TYPE_CONTAINER& );
         template< typename TYPE_CONTAINER >
            bool get_stl_associative_container( TYPE_CONTAINER& );

      /*****************************************
       *
       * Erase methods
       *
       ****************************************/
      public:
         void erase( const size_t size, const size_t offset = 0 );

      public:
         size_t size( ) const;
         size_t capacity( ) const;
         void dump( ) const;
         bool is_linear( const void*& pointer, size_t& size ) const;
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



   /*****************************************
    *
    * Push methods
    *
    ****************************************/
   template< typename TYPE >
   bool ByteStream::push( const std::optional< TYPE >& optional )
   {
      if( false == push( std::nullopt != optional ) )
         return false;

      if( std::nullopt != optional )
         if( false == push( optional.value( ) ) )
            return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::push( const std::vector< TYPE >& vector )
   {
      return push_stl_container( vector );
   }

   template< typename TYPE >
   bool ByteStream::push( const std::list< TYPE >& list )
   {
      return push_stl_container( list );
   }

   template< typename TYPE_FIRST, typename TYPE_SECOND >
   bool ByteStream::push( const std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
   {
      if( false == push( pair.first ) )
         return false;
      if( false == push( pair.second ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::push( const std::set< TYPE >& set )
   {
      return push_stl_associative_container( set );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   bool ByteStream::push( const std::map< TYPE_KEY, TYPE_VALUE >& map )
   {
      return push_stl_associative_container( map );
   }

   template< typename TYPE >
   typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      return push( static_cast< const void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      using ENUM_TYPE = std::underlying_type_t< TYPE >;
      return push( static_cast< ENUM_TYPE >( value ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteStream::push( const TYPE& value )
   {
      return value.to_stream( *this );
   }

   template< typename ... TYPES >
   bool ByteStream::push( const TYPES& ... values )
   {
      bool result = true;
      (void)std::initializer_list< int >{ ( result &= push( values ), 0 )... };

      return result;
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
   template< typename TYPE >
   bool ByteStream::pop( std::optional< TYPE >& optional )
   {
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
   bool ByteStream::pop( std::vector< TYPE >& vector )
   {
      return pop_stl_container( vector );
   }

   template< typename TYPE >
   bool ByteStream::pop( std::list< TYPE >& list )
   {
      return pop_stl_container( list );
   }

   template< typename TYPE_FIRST, typename TYPE_SECOND >
   bool ByteStream::pop( std::pair< TYPE_FIRST, TYPE_SECOND >& pair )
   {
      if( false == pop( pair.first ) )
         return false;
      if( false == pop( pair.second ) )
         return false;

      return true;
   }

   template< typename TYPE >
   bool ByteStream::pop( std::set< TYPE >& set )
   {
      return pop_stl_associative_container( set );
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   bool ByteStream::pop( std::map< TYPE_KEY, TYPE_VALUE >& map )
   {
      return pop_stl_associative_container( map );
   }

   template< typename TYPE >
   typename std::enable_if_t< __INTEGRAL_TYPE__( TYPE ) || __FLOATING_POINT_TYPE__( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      return pop( static_cast< const void* >( &value ), sizeof( TYPE ) );
   }

   template< typename TYPE >
   typename std::enable_if_t< __ENUM_TYPE__( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      using ENUM_TYPE = std::underlying_type_t< TYPE >;
      ENUM_TYPE _value;
      if( false == pop( _value ) )
         return false;
      value = static_cast< TYPE >( _value );

      return true;
   }

   template< typename TYPE >
   typename std::enable_if_t< __REST_TYPES__( TYPE ), bool >
   ByteStream::pop( TYPE& value )
   {
      return value.from_stream( *this );
   }

   template< typename ... TYPES >
   bool ByteStream::pop( TYPES& ... values )
   {
      bool result = true;
      (void)std::initializer_list< int >{ ( result &= pop( values ), 0 )... };

      return result;
   }

   template< typename TYPE_CONTAINER >
   bool ByteStream::pop_stl_container( TYPE_CONTAINER& container )
   {
      size_t size = 0;
      if( false == pop( size ) )
         return false;

      for( size_t index = 0; index < size; ++index )
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
      size_t size = 0;
      if( false == pop( size ) )
         return false;

      for( size_t index = 0; index < size; ++index )
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
    * Get methods
    *
    ****************************************/
   template< typename ... TYPES >
   bool ByteStream::get( TYPES& ... values )
   {
      m_buffer.state_save( );
      bool result = true;
      (void)std::initializer_list< int >{ ( result &= pop( values ), 0 )... };
      m_buffer.state_restore( );

      return result;
   }



   /*****************************************
    *
    * Other methods
    *
    ****************************************/
   inline
   size_t ByteStream::size( ) const
   {
      return m_buffer.size( );
   }

   inline
   size_t ByteStream::capacity( ) const
   {
      return m_buffer.capacity( );
   }

   inline
   void ByteStream::dump( ) const
   {
      return m_buffer.dump( );
   }

   inline
   bool ByteStream::is_linear( const void*& pointer, size_t& size ) const
   {
      return m_buffer.is_linear( pointer, size );
   }

   inline
   void ByteStream::erase( const size_t size, const size_t offset )
   {
      m_buffer.pop_front( size );
   }






} // namespace base
