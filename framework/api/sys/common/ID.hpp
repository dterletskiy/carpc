#pragma once



#include <cstdint>
#include <limits>
#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/dsi/Types.hpp"



namespace base {

   using tValueID = std::uint64_t;
   const tValueID ValueInvalidID = std::numeric_limits< tValueID >::max( );

   template< typename T >
   class TID
   {
      public:
         using tID = TID< T >;
         using TYPE = tValueID;

         static const tID generate( )
         {
            static tID id( 0 );
            return ++id;
         }
         static const tID& invalid( )
         {
            static tID id;
            return id;
         }

      public:
         TID( const tValueID& value = ValueInvalidID ) : m_value( value ) { }
         TID( const tID& other ) : m_value( other.m_value ) { }
         ~TID( ) = default;

      public:
         bool to_stream( dsi::tByteStream& stream ) const { return stream.push( m_value ); }
         bool from_stream( dsi::tByteStream& stream ) { return stream.pop( m_value ); }

      public:
         tID& operator=( const tID& other ) { m_value = other.m_value; return *this; }
         bool operator==( const tID& other ) const { return m_value == other.m_value; }
         bool operator!=( const tID& other ) const { return m_value != other.m_value; }
         bool operator<( const tID& other ) const { return m_value < other.m_value; }
         bool operator>( const tID& other ) const { return m_value > other.m_value; }
         bool operator<=( const tID& other ) const { return m_value <= other.m_value; }
         bool operator>=( const tID& other ) const { return m_value >= other.m_value; }

         tID operator++( ) { return tID( ++m_value ); }
         tID operator++( int ) { return tID( m_value++ ); }
         tID operator--( ) { return tID( --m_value ); }
         tID operator--( int ) { return tID( m_value-- ); }

         operator tValueID( ) const { return m_value; }

      public:
         const tValueID& value( ) const { return m_value; }
         bool is_valid( ) const { return m_value != ValueInvalidID; }
         const std::string name( ) const { return base::format_string( "0x", std::hex, m_value ); }
      private:
         tValueID m_value = ValueInvalidID;

      public:
   };

} // namespace base



// Example
#if 0

   class A
   {
      public:
         using ID = base::TID< A >;

      public:
         A( ) : m_id( ID::generate( ) ) { }
         ~A( ) = default;

         const ID id( ) const { return m_id; }

      private:
         ID m_id;
   };

   class B
   {
      public:
         using ID = base::TID< B >;

      public:
         B( ) : m_id( ID::generate( ) ) { }
         ~B( ) = default;

         const ID id( ) const { return m_id; }

      private:
         ID m_id;
   };



   const int main( int argc, char** argv )
   {
      A a1, a2, a3;
      B b1, b2, b3;

      DBG_MSG( "a1: %s", a1.id( ).name( ).c_str( ) );
      DBG_MSG( "a1: %zu", (A::ID::TYPE)(a1.id( )) );
      DBG_MSG( "a2: %zu", (A::ID::TYPE)(a2.id( )) );
      DBG_MSG( "a3: %zu", (A::ID::TYPE)(a3.id( )) );

      DBG_MSG( "b1: %zu", (B::ID::TYPE)(b1.id( )) );
      DBG_MSG( "b2: %zu", (B::ID::TYPE)(b2.id( )) );
      DBG_MSG( "b3: %zu", (B::ID::TYPE)(b3.id( )) );

      {
         A::ID id_a = A::ID::generate( );
         // B::ID id_b = id_a;
         DBG_MSG( "id_a: %s", id_a.name( ).c_str( ) );
      }

      {
         A::ID id_a = A::ID::invalid( );
         DBG_MSG( "id_a: %s", id_a.name( ).c_str( ) );
      }

      return 0;
   }


#endif