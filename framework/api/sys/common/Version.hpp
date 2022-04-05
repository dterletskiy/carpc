#pragma once

#include <cstdint>
#include <limits>
#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/common/IPC.hpp"



namespace carpc {

   template< typename T, typename V = std::uint8_t >
   class TVersion
   {
      public:
         using tVersion = TVersion< T, V >;
         using OBJECT_TYPE = T;
         using VALUE_TYPE = V;

         static constexpr VALUE_TYPE min_value     = std::numeric_limits< VALUE_TYPE >::min( );
         static constexpr VALUE_TYPE max_value     = std::numeric_limits< VALUE_TYPE >::max( );
         static constexpr VALUE_TYPE invalid_value = max_value;
         static constexpr VALUE_TYPE zero_value    = 0;

         static const tVersion min;
         static const tVersion max;
         static const tVersion invalid;
         static const tVersion zero;

      public:
         TVersion( ) = default;
         TVersion( const VALUE_TYPE& major, const VALUE_TYPE& minor, const VALUE_TYPE& patch )
            : m_major( value )
            , m_minor( value )
            , m_patch( value )
         { }
         TVersion( const tVersion& other )
            : m_major( other.m_major )
            , m_minor( other.m_minor )
            , m_patch( other.m_patch )
         { }
         ~TVersion( ) = default;

      public:
         bool to_stream( ipc::tStream& stream ) const
         {
            return ipc::serialize( stream, m_major, m_minor, m_patch );
         }
         bool from_stream( ipc::tStream& stream )
         {
            return ipc::deserialize( stream, m_major, m_minor, m_patch );
         }

      public:
         tVersion& operator=( const tVersion& other )
         {
            m_major = other.m_major;
            m_minor = other.m_minor;
            m_patch = other.m_patch;
            return *this;
         }
         bool operator==( const tVersion& other ) const
         {
            return m_major == other.m_major && m_minor == other.m_minor && m_patch == other.m_patch;
         }
         bool operator!=( const tVersion& other ) const
         {
            return m_major != other.m_major || m_minor != other.m_minor || m_patch != other.m_patch;
         }
         bool operator<( const tVersion& other ) const
         {
            if( m_major < other.m_major )
               return true;
            if( m_minor < other.m_minor )
               return true;
            return m_patch < other.m_patch;
         }
         bool operator>( const tVersion& other ) const
         {
            if( m_major > other.m_major )
               return true;
            if( m_minor > other.m_minor )
               return true;
            return m_patch > other.m_patch;
         }
         bool operator<=( const tVersion& other ) const
         {
            return !( *this > other );
         }
         bool operator>=( const tVersion& other ) const
         {
            return !( *this < other );
         }

      public:
         const VALUE_TYPE& major( ) const
         {
            return m_major;
         }
         const VALUE_TYPE& minor( ) const
         {
            return m_minor;
         }
         const VALUE_TYPE& patch( ) const
         {
            return m_patch;
         }

         tVersion& inc_major( )
         {
            ++m_major;
            m_minor = zero_value;
            m_patch = zero_value;
            return *this;
         }
         tVersion& inc_minor( )
         {
            ++m_minor;
            m_patch = zero_value;
            return *this;
         }
         tVersion& inc_patch( )
         {
            ++m_patch;
            return *this;
         }

         const std::string name( ) const
         {
            return carpc::format_string( m_major, ".", m_minor, ".", m_patch );
         }
      private:
         VALUE_TYPE m_major = zero_value;
         VALUE_TYPE m_minor = zero_value;
         VALUE_TYPE m_patch = zero_value;
   };



   template< typename T, typename V >
      const TVersion< T, V > TVersion< T, V >::min{ min_value };
   template< typename T, typename V >
      const TVersion< T, V > TVersion< T, V >::max{ max_value };
   template< typename T, typename V >
      const TVersion< T, V > TVersion< T, V >::invalid{ invalid_value };
   template< typename T, typename V >
      const TVersion< T, V > TVersion< T, V >::zero{ zero_value };

} // namespace carpc
