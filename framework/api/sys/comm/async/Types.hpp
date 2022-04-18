#pragma once

#include <string>
#include <type_traits>
#include <functional>

#include "api/sys/comm/Types.hpp"
#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/helpers/functions/generate.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BASE_ASYNC"







// Based on
// link: https://stackoverflow.com/questions/25492589/can-i-use-sfinae-to-selectively-define-a-member-variable-in-a-template-class
namespace __private_carpc_async_v1__ {

   template< typename TYPE >
   class TBaseAsyncTypeID
   {
      public:
         TBaseAsyncTypeID( ) = default;
         TBaseAsyncTypeID( const TYPE& _value ) : m_value( _value )
         { }
         TBaseAsyncTypeID( const TBaseAsyncTypeID< TYPE >& _other )
            : m_value( _other.m_value )
         { }
         ~TBaseAsyncTypeID( ) = default;

      public:
         bool operator==( const TBaseAsyncTypeID< TYPE >& type_id ) const
         {
            return m_value == type_id.m_value;
         }
         bool operator!=( const TBaseAsyncTypeID< TYPE >& type_id ) const
         {
            return m_value != type_id.m_value;
         }
         bool operator<( const TBaseAsyncTypeID< TYPE >& type_id ) const
         {
            return m_value < type_id.m_value;
         }

      public:
         const bool to_stream( carpc::ipc::tStream& stream ) const
         {
            return carpc::ipc::serialize( stream, m_value );
         }
         const bool from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, m_value );
         }

      public:
         const TYPE& value( ) const
         {
            return m_value;
         }
      protected:
         TYPE m_value;
   };



   template< typename TYPE >
   class TAsyncTypeID : public TBaseAsyncTypeID< TYPE > { };



   template< >
   class TAsyncTypeID< std::string > : public TBaseAsyncTypeID< std::string >
   {
      using TYPE = std::string;

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
            static std::string generate( )
            {
               return carpc::generate::name< T >( );
            }

         const std::string& name( ) const
         {
            return TBaseAsyncTypeID< TYPE >::m_value;
         }
         const char* c_str( ) const
         {
            return TBaseAsyncTypeID< TYPE >::m_value.c_str( );
         }
   };



   template< >
   class TAsyncTypeID< std::size_t > : public TBaseAsyncTypeID< std::size_t >
   {
      using TYPE = std::size_t;

      public:
         TAsyncTypeID( ) = default;
         TAsyncTypeID( const TYPE& _value )
            : TBaseAsyncTypeID< TYPE >( _value )
            , m_name( carpc::format_string( "0x", std::hex, _value ) )
         { }
         TAsyncTypeID( const TAsyncTypeID< TYPE >& _other )
            : TBaseAsyncTypeID< TYPE >( _other.m_value )
            , m_name( _other.m_name )
         { }
         ~TAsyncTypeID( ) = default;

      public:
         const bool to_stream( carpc::ipc::tStream& stream ) const
         {
            return carpc::ipc::serialize( stream, TAsyncTypeID< TYPE >::m_value, m_name );
         }
         const bool from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, TAsyncTypeID< TYPE >::m_value, m_name );
         }

      public:
         template< typename T >
            static std::size_t generate( )
            {
               const std::string _name = carpc::generate::name< T >( );
               const std::size_t _hash_code = carpc::generate::hash< T >( );
               SYS_DBG( "async type_id: %p => %s", (void*)_hash_code, _name.c_str( ) );
               return _hash_code;
            }

         const std::string& name( ) const
         {
            return m_name;
         }
         const char* c_str( ) const
         {
            return m_name.c_str( );
         }

      private:
         std::string m_name = "NoNameTypeID";
   };

}

// This variant in not prefereble but interesting from implementation point of view of methods "name" and "generate"
namespace __private_carpc_async_v2__ {

   template< typename T >
   class TAsyncTypeID
   {
      public:
         TAsyncTypeID( ) = default;
         TAsyncTypeID( const T& _value )
            : m_value( _value )
         { }
         TAsyncTypeID( const TAsyncTypeID< T >& _other )
            : m_value( _other.m_value )
         { }
         ~TAsyncTypeID( ) = default;

      public:
         // void operator=( const T& _value ) { m_value = _value; }
         // void operator=( const TAsyncTypeID< T >& type_id ) { m_value = type_id.m_value; }
         bool operator==( const TAsyncTypeID< T >& type_id ) const
         {
            return m_value == type_id.m_value;
         }
         bool operator!=( const TAsyncTypeID< T >& type_id ) const
         {
            return m_value != type_id.m_value;
         }
         bool operator<( const TAsyncTypeID< T >& type_id ) const
         {
            return m_value < type_id.m_value;
         }

      public:
         const bool to_stream( carpc::ipc::tStream& stream ) const
         {
            return carpc::ipc::serialize( stream, m_value );
         }
         const bool from_stream( carpc::ipc::tStream& stream )
         {
            return carpc::ipc::deserialize( stream, m_value );
         }

      public:
         template< typename TYPE >
            static T generate( )
            {
               return generate_t< TYPE >( );
            }
      private:
         template< typename TYPE, typename V = T >
            static std::enable_if_t< std::is_same_v< V, std::size_t >, T >
               generate_t( )
               {
                  return carpc::generate::hash< T >( );
               }
         template< typename TYPE, typename V = T >
            static std::enable_if_t< std::is_same_v< V, std::string >, T >
               generate_t( )
               {
                  return carpc::generate::name< T >( );
               }

      public:
         const std::string name( ) const
         {
            return name_t( );
         }
      private:
         template< typename V = T >
            std::enable_if_t< std::is_same_v< V, std::size_t >, const std::string >
               name_t( ) const
               {
                  return format_string( m_value );
               }
         template< typename V = T >
            std::enable_if_t< std::is_same_v< V, std::string >, const std::string >
               name_t( ) const
               {
                  return m_value;
               }

      public:
         const T& value( ) const
         {
            return m_value;
         }
      private:
         T m_value;
   };

}

namespace carpc::async {

   // using tAsyncTypeID = std::string;
   using tAsyncTypeID = __private_carpc_async_v1__::TAsyncTypeID< std::size_t >;
   // using tAsyncTypeID = __private_carpc_async_v1__::TAsyncTypeID< std::string >;

   enum class eAsyncType : std::uint8_t { EVENT, RUNNABLE, CALLABLE };
   const char* c_str( const eAsyncType );

} // namespace carpc::async



#undef CLASS_ABBR
