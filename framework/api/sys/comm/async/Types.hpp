#pragma once

#include "api/sys/common/Includes.hpp"
#include "api/sys/dsi/Types.hpp"
#include "api/sys/helpers/functions/format.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BASE_ASYNC"



// Based on
// link: https://stackoverflow.com/questions/25492589/can-i-use-sfinae-to-selectively-define-a-member-variable-in-a-template-class
namespace base_v1 {

   template< typename TYPE >
   class TBaseAsyncTypeID
   {
      public:
         TBaseAsyncTypeID( ) = default;
         TBaseAsyncTypeID( const TYPE& _value ) : m_value( _value ) { }
         TBaseAsyncTypeID( const TBaseAsyncTypeID< TYPE >& _other ) : m_value( _other.m_value ) { }
         ~TBaseAsyncTypeID( ) = default;

      public:
         bool operator==( const TBaseAsyncTypeID< TYPE >& type_id ) const { return m_value == type_id.m_value; }
         bool operator!=( const TBaseAsyncTypeID< TYPE >& type_id ) const { return m_value != type_id.m_value; }
         bool operator<( const TBaseAsyncTypeID< TYPE >& type_id ) const { return m_value < type_id.m_value; }

      public:
         const bool to_stream( base::dsi::tByteStream& stream ) const { return stream.push( m_value ); }
         const bool from_stream( base::dsi::tByteStream& stream ) { return stream.pop( m_value ); }

      public:
         const TYPE& value( ) const { return m_value; }
      protected:
         TYPE m_value;
   };



   template< typename TYPE, typename Enable = void >
   class TAsyncTypeID : public TBaseAsyncTypeID< TYPE > { };



   template< typename TYPE >
   class TAsyncTypeID< TYPE, std::enable_if_t< std::is_same_v< TYPE, std::string > > > : public TBaseAsyncTypeID< TYPE >
   {
      public:
         TAsyncTypeID( ) = default;
         TAsyncTypeID( const TYPE& _value )
            : TBaseAsyncTypeID< TYPE >( _value )
         { }
         TAsyncTypeID( const TAsyncTypeID< TYPE >& _other )
            : TBaseAsyncTypeID< TYPE >( _other.m_value )
         { }
         ~TAsyncTypeID( ) = default;

      public:
         template< typename T >
            static std::string generate( ) { return typeid( T ).name( ); }

         const std::string& name( ) const { return TBaseAsyncTypeID< TYPE >::m_value; }
         const char* c_str( ) const { return TBaseAsyncTypeID< TYPE >::m_value.c_str( ); }
   };



   template< typename TYPE >
   class TAsyncTypeID< TYPE, std::enable_if_t< std::is_same_v< TYPE, std::size_t > > > : public TBaseAsyncTypeID< TYPE >
   {
      public:
         TAsyncTypeID( ) = default;
         TAsyncTypeID( const TYPE& _value )
            : TBaseAsyncTypeID< TYPE >( _value )
            , m_name( base::format_string( "0x", std::hex, _value ) )
         { }
         TAsyncTypeID( const TAsyncTypeID< TYPE >& _other )
            : TBaseAsyncTypeID< TYPE >( _other.m_value )
            , m_name( base::format_string( "0x", std::hex, _other.m_value ) )
         { }
         ~TAsyncTypeID( ) = default;

      public:
         const bool to_stream( base::dsi::tByteStream& stream ) const { return stream.push( TAsyncTypeID< TYPE >::m_value, m_name ); }
         const bool from_stream( base::dsi::tByteStream& stream ) { return stream.pop( TAsyncTypeID< TYPE >::m_value, m_name ); }

      public:
         template< typename T >
            static std::size_t generate( )
            {
               const std::string _name = base::format_string( std::hex, typeid( T ).name( ) );
               const std::size_t _hash_code = typeid( T ).hash_code( );
               SYS_MSG( "async typeid: %p => %s", (void*)_hash_code, _name.c_str( ) );
               return typeid( T ).hash_code( );
            }

         const std::string& name( ) const { return m_name; }
         const char* c_str( ) const { return m_name.c_str( ); }

      private:
         std::string m_name = "NoNameTypeID";
   };

}

// This variant in not prefereble but interesting from implementation point of view of methods "name" and "generate"
namespace base_v2 {

   template< typename T >
   class TAsyncTypeID
   {
      public:
         TAsyncTypeID( ) = default;
         TAsyncTypeID( const T& _value ) : m_value( _value ) { }
         TAsyncTypeID( const TAsyncTypeID< T >& _other ) : m_value( _other.m_value ) { }
         ~TAsyncTypeID( ) = default;

      public:
         // void operator=( const T& _value ) { m_value = _value; }
         // void operator=( const TAsyncTypeID< T >& type_id ) { m_value = type_id.m_value; }
         bool operator==( const TAsyncTypeID< T >& type_id ) const { return m_value == type_id.m_value; }
         bool operator!=( const TAsyncTypeID< T >& type_id ) const { return m_value != type_id.m_value; }
         bool operator<( const TAsyncTypeID< T >& type_id ) const { return m_value < type_id.m_value; }

      public:
         const bool to_stream( base::dsi::tByteStream& stream ) const { return stream.push( m_value ); }
         const bool from_stream( base::dsi::tByteStream& stream ) { return stream.pop( m_value ); }

      public:
         template< typename TYPE >
            static T generate( ) { return generate_t< TYPE >( ); }
      private:
         template< typename TYPE, typename V = T >
            static std::enable_if_t< std::is_same_v< V, std::size_t >, T >
               generate_t( ) { return typeid( TYPE ).hash_code( ); }
         template< typename TYPE, typename V = T >
            static std::enable_if_t< std::is_same_v< V, std::string >, T >
               generate_t( ) { return typeid( TYPE ).name( ); }

      public:
         const std::string name( ) const { return name_t( ); }
      private:
         template< typename V = T >
            std::enable_if_t< std::is_same_v< V, std::size_t >, const std::string >
               name_t( ) const { return format_string( m_value ); }
         template< typename V = T >
            std::enable_if_t< std::is_same_v< V, std::string >, const std::string >
               name_t( ) const { return m_value; }

      public:
         const T& value( ) const { return m_value; }
      private:
         T m_value;
   };

}

namespace base::async {

   // using tAsyncTypeID = std::string;
   using tAsyncTypeID = base_v1::TAsyncTypeID< std::size_t >;
   // using tAsyncTypeID = base_v1::TAsyncTypeID< std::string >;

   using NoServiceType = void;
   #define IS_IPC_EVENT ( false == std::is_same_v< tService, NoServiceType > )

   enum class eAsyncType : size_t { EVENT, RUNNABLE, UNDEFINED };
   const char* c_str( const eAsyncType );

} // namespace base::async



#undef CLASS_ABBR