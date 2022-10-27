#pragma once

#include <sstream>
#include <string>
#include <limits>
#include <type_traits>



namespace carpc::converter {

   template< typename T >
      std::enable_if_t< std::is_integral_v< T > || std::is_floating_point_v< T >, bool >
         from_string( const std::string& value_str, T& value, std::size_t* pos = nullptr, int base = 10 )
   {
      T min_value = std::numeric_limits< T >::min( );
      T max_value = std::numeric_limits< T >::max( );
      MSG_DBG(
            "converting '%s' (min = %s / max = %s)",
            value_str.c_str( ),
            std::to_string( min_value ).c_str( ),
            std::to_string( max_value ).c_str( )
         );

      bool result = true;

      try
      {
         #if 0 // Depricated implementation
            // Processing unsigned integral values
            if( std::is_same_v< unsigned short int, T > )
            {
               unsigned long int tmp_value = std::stoul( value_str, pos, base );
               if( tmp_value > max_value || tmp_value < min_value )
                  throw std::out_of_range( "stous" );
               value = static_cast< T >( tmp_value );
            }
            else if( std::is_same_v< unsigned int, T > )
            {
               unsigned long int tmp_value = std::stoul( value_str, pos, base );
               if( tmp_value > max_value || tmp_value < min_value )
                  throw std::out_of_range( "stoui" );
               value = static_cast< T >( tmp_value );
            }
            else if( std::is_same_v< unsigned long int, T > )
            {
               value = std::stoul( value_str, pos, base );
            }
            else if( std::is_same_v< unsigned long long int, T > )
            {
               value = std::stoull( value_str, pos, base );
            }
            // Processing signed integral values
            else if( std::is_same_v< short int, T > )
            {
               int tmp_value = std::stoi( value_str, pos, base );
               if( tmp_value > max_value || tmp_value < min_value )
                  throw std::out_of_range( "stos" );
               value = static_cast< T >( tmp_value );
            }
            else if( std::is_same_v< int, T > )
            {
               value = std::stoi( value_str, pos, base );
            }
            else if( std::is_same_v< long int, T > )
            {
               value = std::stol( value_str, pos, base );
            }
            else if( std::is_same_v< long long int, T > )
            {
               value = std::stoll( value_str, pos, base );
            }
         #endif
         // Processing integral values
         if( std::is_integral_v< T > )
         {
            // Processing signed integral values
            if( std::is_signed_v< T > )
            {
               long long int tmp_value = std::stoll( value_str, pos, base );
               if( tmp_value < min_value )
               {
                  std::string error =
                       std::string{ "value( " }
                     + std::to_string( tmp_value )
                     + std::string{ " ) < min( " }
                     + std::to_string( min_value )
                     + std::string{ " )" };
                  throw std::out_of_range( error );
               }
               else if( tmp_value > max_value )
               {
                  std::string error =
                       std::string{ "value( " }
                     + std::to_string( tmp_value )
                     + std::string{ " ) > max( " }
                     + std::to_string( max_value )
                     + std::string{ " )" };
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
                  std::string error =
                       std::string{ "value( " }
                     + std::to_string( tmp_value )
                     + std::string{ " ) < min( " }
                     + std::to_string( min_value )
                     + std::string{ " )" };
                  throw std::out_of_range( error );
               }
               else if( tmp_value > max_value )
               {
                  std::string error =
                       std::string{ "value( " }
                     + std::to_string( tmp_value )
                     + std::string{ " ) > max( " }
                     + std::to_string( max_value )
                     + std::string{ " )" };
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
