#pragma once

#include <string>
#include "api/sys/common/IPC.hpp"



namespace carpc {

   template< typename T, typename V = std::string >
   class TName
   {
      public:
         using tName = TName< T, V >;
         using OBJECT_TYPE = T;
         using VALUE_TYPE = V;

         static const VALUE_TYPE invalid_value;
         static const tName invalid;

      public:
         TName( const VALUE_TYPE& value = invalid_value )
            : m_value( value )
         { }
         TName( const tName& other )
            : m_value( other.m_value )
         { }
         ~TName( ) = default;

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
         tName& operator=( const tName& other )
         {
            m_value = other.m_value;
            return *this;
         }
         bool operator==( const tName& other ) const
         {
            return other.m_value == m_value;
         }
         bool operator==( const VALUE_TYPE& value ) const
         {
            return value == m_value;
         }
         bool operator!=( const tName& other ) const
         {
            return other.m_value != m_value;
         }
         bool operator!=( const VALUE_TYPE& value ) const
         {
            return value != m_value;
         }
         bool operator<( const tName& other ) const
         {
            return m_value < other.m_value;
         }
         bool operator<( const VALUE_TYPE& value ) const
         {
            return m_value < value;
         }
         bool operator>( const tName& other ) const
         {
            return m_value > other.m_value;
         }
         bool operator>( const VALUE_TYPE& value ) const
         {
            return m_value > value;
         }
         bool operator<=( const tName& other ) const
         {
            return m_value <= other.m_value;
         }
         bool operator<=( const VALUE_TYPE& value ) const
         {
            return m_value <= value;
         }
         bool operator>=( const tName& other ) const
         {
            return m_value >= other.m_value;
         }
         bool operator>=( const VALUE_TYPE& value ) const
         {
            return m_value >= value;
         }

      public:
         const VALUE_TYPE& value( ) const
         {
            return m_value;
         }
         const char* const c_str( ) const
         {
            return m_value.c_str( );
         }
         bool is_valid( ) const
         {
            return m_value != invalid_value;
         }
         bool is_invalid( ) const
         {
            return m_value == invalid_value;
         }
      private:
         VALUE_TYPE m_value = invalid_value;
   };



   template< typename T, typename V >
      const typename TName< T, V >::VALUE_TYPE TName< T, V >::invalid_value{ "NoName" };
   template< typename T, typename V >
      const TName< T, V > TName< T, V >::invalid{ invalid_value };

} // namespace carpc
