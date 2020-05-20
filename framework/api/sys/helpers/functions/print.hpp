#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   /****************************************************************************************************
    * 
    * Print std containers and types
    * 
    ***************************************************************************************************/
   void print( const void* p_buffer, const size_t size, const bool is_new_line = false );
   void print( const std::string& string, const bool is_new_line = false );

   template< typename TYPE >
   void print( const TYPE& value, const bool is_new_line = false )
   {
      std::cout << value;

      if( is_new_line ) std::cout << std::endl;
   }


   template< typename TYPE >
   void print( const std::list< TYPE >& list, const bool is_new_line = false )
   {
      std::cout << "{ " ;
      for ( const auto& value : list )
      {
         print( value );
         std::cout << " ";
      }
      std::cout << "}";

      if( is_new_line ) std::cout << std::endl;
   }

   template< typename TYPE >
   void print( const std::vector< TYPE >& vector, const bool is_new_line = false )
   {
      std::cout << "{ " ;
      for ( const auto& value : vector )
      {
         print( value );
         std::cout << " ";
      }
      std::cout << "}";

      if( is_new_line ) std::cout << std::endl;
   }

   template< typename TYPE >
   void print( const std::set< TYPE >& set, const bool is_new_line = false )
   {
      std::cout << "{ " ;
      for ( const auto& value : set )
      {
         print( value );
         std::cout << " ";
      }
      std::cout << "}";

      if( is_new_line ) std::cout << std::endl;
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   void print( const std::pair< TYPE_KEY, TYPE_VALUE >& pair, const bool is_new_line = false )
   {
      std::cout << "{ " ;
      print( pair.first );
      std::cout << " => ";
      print( pair.second );
      std::cout << " }";

      if( is_new_line ) std::cout << std::endl;
   }

   template< typename TYPE_KEY, typename TYPE_VALUE >
   void print( const std::map< TYPE_KEY, TYPE_VALUE >& map, const bool is_new_line = false )
   {
      std::cout << "{ " ;
      for ( const auto& pair : map )
      {
         print( pair );
         std::cout << " ";
      }
      std::cout << " }";

      if( is_new_line ) std::cout << std::endl;
   }

} // namespace base
