#pragma once

#include "carpc/helpers/functions/is_base.hpp"



namespace carpc {

   /****************************************************************************************************
    * 
    * link: https://stackoverflow.com/a/28796458
    * 
    * This checks is class Test is the specification of template class Ref
    * Usage:
    *    
    *    int main( )
    *    {
    *       using vector_int = std::vector< int >;
    *       using vector_float = std::vector< float >;
    *       using list_int = std::list< int >;
    *       using list_float = std::list< float >;
    * 
    * 
    *       MSG_DBG( "comparing 'vector_int' and 'std::vector': %s",
    *             is_specialization< vector_int, std::vector >::value ? "true" : "false"
    *          );
    *       MSG_DBG( "comparing 'vector_float' and 'std::vector': %s",
    *             is_specialization< vector_float, std::vector >::value ? "true" : "false"
    *          );
    *       MSG_DBG( "comparing 'list_int' and 'std::vector': %s",
    *             is_specialization< list_int, std::vector >::value ? "true" : "false"
    *          );
    *       MSG_DBG( "comparing 'list_float' and 'std::vector': %s",
    *             is_specialization< list_float, std::vector >::value ? "true" : "false"
    *          );
    * 
    *       MSG_DBG( "comparing 'vector_int' and 'std::list': %s",
    *             is_specialization< vector_int, std::list >::value ? "true" : "false"
    *          );
    *       MSG_DBG( "comparing 'vector_float' and 'std::list': %s",
    *             is_specialization< vector_float, std::list >::value ? "true" : "false"
    *          );
    *       MSG_DBG( "comparing 'list_int' and 'std::list': %s",
    *             is_specialization< list_int, std::list >::value ? "true" : "false"
    *          );
    *       MSG_DBG( "comparing 'list_float' and 'std::list': %s",
    *             is_specialization< list_float, std::list >::value ? "true" : "false"
    *          );
    * 
    * 
    * 
    *       return 0;
    *    }
    * 
    ***************************************************************************************************/

   template< typename Test, template< typename... > class Ref >
   struct is_specialization: std::false_type { };

   template< template< typename... > class Ref, typename... Args >
   struct is_specialization< Ref< Args... >, Ref >: std::true_type { } ;

} // namespace carpc
