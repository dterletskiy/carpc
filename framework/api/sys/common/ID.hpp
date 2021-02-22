#pragma once

#include <cstdint>
#include <limits>
#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/dsi/Types.hpp"



namespace base {

   template< typename T >
   class TID
   {
      public:
         using tID = TID< T >;
         using VALUE_TYPE = std::uint64_t;
         using OBJECT_TYPE = T;

         static constexpr VALUE_TYPE min_value     = std::numeric_limits< VALUE_TYPE >::min( );
         static constexpr VALUE_TYPE max_value     = std::numeric_limits< VALUE_TYPE >::max( );
         static constexpr VALUE_TYPE invalid_value = max_value;
         static constexpr VALUE_TYPE zero_value    = 0;

         static const tID min;
         static const tID max;
         static const tID invalid;
         static const tID zero;

         static const tID generate( )
         {
            static tID id( zero_value );
            return ++id;
         }

      public:
         TID( const VALUE_TYPE& value = invalid_value )
            : m_value( value )
         { }
         TID( const tID& other )
            : m_value( other.m_value )
         { }
         ~TID( ) = default;

      public:
         bool to_stream( dsi::tByteStream& stream ) const
         {
            return stream.push( m_value );
         }
         bool from_stream( dsi::tByteStream& stream )
         {
            return stream.pop( m_value );
         }

      public:
         tID& operator=( const tID& other )
         {
            m_value = other.m_value;
            return *this;
         }
         bool operator==( const tID& other ) const
         {
            return m_value == other.m_value;
         }
         bool operator!=( const tID& other ) const
         {
            return m_value != other.m_value;
         }
         bool operator<( const tID& other ) const
         {
            return m_value < other.m_value;
         }
         bool operator>( const tID& other ) const
         {
            return m_value > other.m_value;
         }
         bool operator<=( const tID& other ) const
         {
            return m_value <= other.m_value;
         }
         bool operator>=( const tID& other ) const
         {
            return m_value >= other.m_value;
         }

         tID operator++( )
         {
            ++m_value;
            return *this;
         }
         tID operator++( int )
         {
            return tID( m_value++ );
         }
         tID operator--( )
         {
            return tID( --m_value );
         }
         tID operator--( int )
         {
            return tID( m_value-- );
         }

         tID operator+( const VALUE_TYPE& value ) const
         {
            return tID( m_value + value );
         }
         tID operator-( const VALUE_TYPE& value ) const
         {
            return tID( m_value - value );
         }

         // operator VALUE_TYPE( ) const { return m_value; }

      public:
         const VALUE_TYPE& value( ) const
         {
            return m_value;
         }
         bool is_valid( ) const
         {
            return m_value != invalid_value;
         }
         bool is_invalid( ) const
         {
            return m_value == invalid_value;
         }
         const std::string name( ) const
         {
            return base::format_string( "0x", std::hex, m_value );
         }
      private:
         VALUE_TYPE m_value = invalid_value;
   };



   template< typename T >
      const TID< T > TID< T >::min{ min_value };
   template< typename T >
      const TID< T > TID< T >::max{ max_value };
   template< typename T >
      const TID< T > TID< T >::invalid{ invalid_value };
   template< typename T >
      const TID< T > TID< T >::zero{ zero_value };


   struct Object { using ID = TID< Object >; };
   using tID = Object::ID;


} // namespace base



// Example
#if 0

   class A
   {
      public:
         using ID = base::TID< A >;

      public:
         A( ) = default;
         ~A( ) = default;

         const ID id( ) const { return m_id; }

      private:
         ID m_id = ID::generate( );
   };

   class B
   {
      public:
         using ID = base::TID< B >;

      public:
         B( ) = default;
         ~B( ) = default;

         const ID id( ) const { return m_id; }

      private:
         ID m_id = ID::generate( );
   };



   const int main( int argc, char** argv )
   {
      A a1, a2, a3;
      B b1, b2, b3;

      MSG_DBG( "a1: %s", a1.id( ).name( ).c_str( ) );
      MSG_DBG( "a1: %zu", (A::ID::TYPE)(a1.id( )) );
      MSG_DBG( "a2: %zu", (A::ID::TYPE)(a2.id( )) );
      MSG_DBG( "a3: %zu", (A::ID::TYPE)(a3.id( )) );

      MSG_DBG( "b1: %zu", (B::ID::TYPE)(b1.id( )) );
      MSG_DBG( "b2: %zu", (B::ID::TYPE)(b2.id( )) );
      MSG_DBG( "b3: %zu", (B::ID::TYPE)(b3.id( )) );

      {
         A::ID id_a = A::ID::generate( );
         // B::ID id_b = id_a;
         MSG_DBG( "id_a: %s", id_a.name( ).c_str( ) );
      }

      {
         A::ID id_a = A::ID::invalid;
         MSG_DBG( "id_a: %s", id_a.name( ).c_str( ) );
      }

      return 0;
   }


#endif