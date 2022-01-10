#pragma once

#include <cstdint>
#include <limits>
#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/common/IPC.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "BASE"



namespace base {

   template< typename T >
   class TPriority
   {
      public:
         using tPriority = TPriority< T >;
         using VALUE_TYPE = T;

         static constexpr T min_value = std::numeric_limits< T >::min( );
         static constexpr T max_value = std::numeric_limits< T >::max( );
         static constexpr T invalid_value = max_value;
         static constexpr T zero_value = 0;

         static const tPriority min;
         static const tPriority max;
         static const tPriority invalid;
         static const tPriority zero;

      public:
         TPriority( ) = default;
         TPriority( const T& value )
            : m_value( value )
         {
            check( );
         }
         TPriority( const tPriority& other )
            : m_value( other.m_value )
         {
            check( );
         }
         ~TPriority( ) = default;

      public:
         bool to_stream( ipc::tStream& stream ) const
         {
            return ipc::serialize( stream, m_value );
         }
         bool from_stream( ipc::tStream& stream )
         {
            return ipc::deserialize( stream, m_value );
         }

      public:
         operator T( ) const
         {
            return m_value;
         }
         operator std::size_t( ) const
         {
            return static_cast< std::size_t >( m_value );
         } // for indexing in vector
         bool operator==( const tPriority& other )
         {
            return m_value == other.m_value;
         }
         bool operator!=( const tPriority& other )
         {
            return m_value != other.m_value;
         }
         bool operator<( const tPriority& other )
         {
            return m_value < other.m_value;
         }
         bool operator>( const tPriority& other )
         {
            return m_value > other.m_value;
         }
         bool operator<=( const tPriority& other )
         {
            return m_value <= other.m_value;
         }
         bool operator>=( const tPriority& other )
         {
            return m_value >= other.m_value;
         }
         tPriority& operator++( )
         {
            ++m_value;
            check( );
            return *this;
         }
         tPriority operator++( int )
         {
            tPriority prio = *this;
            ++m_value;
            check( );
            return prio;
         }
         tPriority& operator--( )
         {
            --m_value;
            check( );
            return *this;
         }
         tPriority operator--( int )
         {
            tPriority prio = *this;
            --m_value;
            check( );
            return prio;
         }

      public:
         void value( const T& _value )
         {
            m_value = _value;
            check( );
         }
         const T& value( ) const
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
            return base::format_string( "0x", std::hex, static_cast< std::size_t >( m_value ) );
         }
      private:
         T m_value = invalid_value;

      private:
         void check( )
         {
            if( max_value < m_value )
            {
               SYS_WRN( "correcting value to max" );
               m_value = max_value;
            }
            else if( min_value > m_value )
            {
               SYS_WRN( "correcting value to min" );
               m_value = min_value;
            }
         }
   };



   template< typename T >
      const TPriority< T > TPriority< T >::min{ min_value };
   template< typename T >
      const TPriority< T > TPriority< T >::max{ max_value };
   template< typename T >
      const TPriority< T > TPriority< T >::invalid{ invalid_value };
   template< typename T >
      const TPriority< T > TPriority< T >::zero{ zero_value };



   using tPriorityTYPE = std::uint16_t;
   using tPriority = TPriority< tPriorityTYPE >;



   enum class ePriority : tPriorityTYPE
   {
      DEFAULT,
      TIMER,
      MAX
   };

   const std::map< ePriority, tPriority > g_priority_map = {
      { ePriority::DEFAULT   , tPriority( 100 ) },
      { ePriority::TIMER     , tPriority( 200 ) },
      { ePriority::MAX       , tPriority( 256 ) }  // Must be last and has max value
   };

   inline
   const tPriority& priority( const ePriority type )
   {
      if( g_priority_map.end( ) == g_priority_map.find( type ) )
      {
         SYS_WRN( "Undefined priority" );
         static const tPriority& dummy = g_priority_map.at( ePriority::DEFAULT );
         return dummy;
      }
      return g_priority_map.at( type );
   }

} // namespace base



#undef CLASS_ABBR
