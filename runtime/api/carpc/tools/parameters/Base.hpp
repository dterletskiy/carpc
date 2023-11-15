#pragma once

#include <memory>

#include "carpc/tools/parameters/Types.hpp"



namespace carpc::tools::parameters {

   class Base
   {
      public:
         using tSptr = std::shared_ptr< Base >;

      public:
         Base( const tMap& map, const std::string& dump_message );
         ~Base( ) = default;

      public:
         bool is_exist( const tParameter& parameter ) const;

         std::pair< tValueOpt, bool > value( const tParameter& parameter ) const;
         template< typename T >
            std::pair< std::optional< T >, bool > value( const tParameter& parameter ) const;

         std::pair< tValue, bool > value_or( const tParameter& parameter, const tValue& default_value ) const;
         template< typename T >
            std::pair< T, bool > value_or( const tParameter& parameter, const T& default_value ) const;

         void print( ) const;

      protected:
         tMap           m_map;
         std::string    m_dump_message = "DUMP";
   };



   template< typename T >
   std::pair< std::optional< T >, bool > Base::value( const tParameter& parameter ) const
   {
      const auto iterator = m_map.find( parameter );
      if( m_map.end( ) == iterator )
         return std::make_pair( invalid_value, false );

      if( invalid_value == iterator->second )
         return std::make_pair( invalid_value, true );

      std::stringstream ss( iterator->second );
      T value;
      ss >> value;

      return std::make_pair( std::make_optional< T >( value ), true );
   }

   template< typename T >
   std::pair< T, bool > Base::value_or( const tParameter& parameter, const T& default_value ) const
   {
      const auto iterator = m_map.find( parameter );
      if( m_map.end( ) == iterator )
         return std::make_pair( default_value, false );

      if( invalid_value == iterator->second )
         return std::make_pair( default_value, true );

      std::stringstream ss( iterator->second.value( ) );
      T value;
      ss >> value;

      return std::make_pair( value, true );
   }

} // namespace carpc::tools::parameters
