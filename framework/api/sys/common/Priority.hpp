#pragma once

#include <cstdint>
#include <limits>
#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/dsi/Types.hpp"

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

         static const tPriority min( )
         {
            static tPriority priority( min_value );
            return priority;
         }
         static const tPriority max( )
         {
            static tPriority priority( max_value );
            return priority;
         }
         static const tPriority invalid( )
         {
            static tPriority priority( invalid_value );
            return priority;
         }
         static const tPriority zero( )
         {
            static tPriority priority( zero_value );
            return priority;
         }

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
         bool to_stream( dsi::tByteStream& stream ) const
         {
            return stream.push( m_value );
         }
         bool from_stream( dsi::tByteStream& stream )
         {
            return stream.pop( m_value );
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
      try
      {
         return g_priority_map.at( type );
      }
      catch( const std::out_of_range& oor )
      {
         SYS_WRN( "Obtaining priority error: %s", oor.what( ) );
         return g_priority_map.at( ePriority::DEFAULT );
      }
   }

} // namespace base



#undef CLASS_ABBR
