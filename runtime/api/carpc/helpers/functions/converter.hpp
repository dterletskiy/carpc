#pragma once

#include <sstream>
#include <string>
#include <limits>
#include <type_traits>



namespace carpc::converter {

   template< typename T >
   const char* const to_string( )
   {
      if( std::is_same_v< char, T > )                 return "char";
      else if( std::is_same_v< short int, T > )       return "short int";
      else if( std::is_same_v< int, T > )             return "int";
      else if( std::is_same_v< long int, T > )        return "long int";
      else if( std::is_same_v< long long int, T > )   return "long long int";
      else if( std::is_same_v< float, T > )           return "float";
      else if( std::is_same_v< double, T > )          return "double";
      else if( std::is_same_v< long double, T > )     return "long double";
      else                                            return "";

      return "";
   }

   # define TYPE_INFO( TYPE ) \
      MSG_DBG( "'%s' -> '%s' (min = %s / max = %s / sizeof = %zu)" \
            , #TYPE \
            , carpc::converter::to_string< TYPE >( ) \
            , std::to_string( std::numeric_limits< TYPE >::min( ) ).c_str( ) \
            , std::to_string( std::numeric_limits< TYPE >::max( ) ).c_str( ) \
            , sizeof( TYPE ) \
         )



   template< typename T >
      std::enable_if_t< std::is_integral_v< T > || std::is_floating_point_v< T >, bool >
         from_string( const std::string& value_str, T& value, std::size_t* pos = nullptr, int base = 10 )
   {
      static const T min_value = std::numeric_limits< T >::min( );
      static const T max_value = std::numeric_limits< T >::max( );
      static const std::string min_value_str = std::to_string( min_value );
      static const std::string max_value_str = std::to_string( max_value );
      MSG_DBG( "converting '%s' (min = %s / max = %s / sizeof = %zu)"
            , value_str.c_str( )
            , min_value_str.c_str( )
            , max_value_str.c_str( )
            , sizeof( T )
         );

      bool result = true;

      try
      {
         // Processing integral values
         if( std::is_integral_v< T > )
         {
            static const std::size_t error_size = 1024;
            static thread_local char error[ error_size ];
            static const char* const error_format_min_lld = "value( %lld ) < min( %s )";
            static const char* const error_format_max_lld = "value( %lld ) > max( %s )";
            static const char* const error_format_min_llu = "value( %llu ) < min( %s )";
            static const char* const error_format_max_llu = "value( %llu ) > max( %s )";

            // Processing signed integral values
            if( std::is_signed_v< T > )
            {
               long long int tmp_value = std::stoll( value_str, pos, base );
               if( tmp_value < min_value )
               {
                  std::size_t size = ::snprintf( error, error_size, error_format_min_lld, tmp_value, min_value_str.c_str( ) );
                  throw std::out_of_range( error );
               }
               else if( tmp_value > max_value )
               {
                  std::size_t size = ::snprintf( error, error_size, error_format_max_lld, tmp_value, max_value_str.c_str( ) );
                  throw std::out_of_range( error );
               }
               value = static_cast< T >( tmp_value );
            }
            // Processing unsigned integral values
            else
            {
               long long unsigned int tmp_value = std::stoull( value_str, pos, base );
               if( tmp_value < min_value )
               {
                  std::size_t size = ::snprintf( error, error_size, error_format_min_llu, tmp_value, min_value_str.c_str( ) );
                  throw std::out_of_range( error );
               }
               else if( tmp_value > max_value )
               {
                  std::size_t size = ::snprintf( error, error_size, error_format_max_llu, tmp_value, max_value_str.c_str( ) );
                  throw std::out_of_range( error );
               }
               value = static_cast< T >( tmp_value );
            }
         }
         // Processing floating point values
         else if( std::is_same_v< float, T > )
         {
            value = std::stof( value_str, pos );
         }
         else if( std::is_same_v< double, T > )
         {
            value = std::stod( value_str, pos );
         }
         else if( std::is_same_v< long double, T > )
         {
            value = std::stold( value_str, pos );
         }
         else
         {
            MSG_WRN( "trying to convert string to unsupported type" );
            result = false;
         }
      }
      catch( std::invalid_argument const& ex )
      {
         MSG_ERR( "std::invalid_argument::what( ): %s", ex.what( ) );
         result = false;
      }
      catch( std::out_of_range const& ex )
      {
         MSG_ERR( "std::out_of_range::what( ): %s", ex.what( ) );
         result = false;
      }

      return result;
   }

   template< typename T >
   bool from_string_ss( const std::string& value_str, T& value )
   {
      bool result = true;

      std::stringstream ss( value_str );
      ss >> value;

      return value;
   }

} // namespace carpc::converter
