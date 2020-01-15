#pragma once

#include <string>
#include <sstream>
#include <initializer_list>
#include "api/sys/common/Types.hpp"



namespace base {



/****************************************************************************************************
 * 
 * Building formated string
 * 
 ***************************************************************************************************/
template< typename ... TYPES >
const std::string format_string( const TYPES ... args )
{
   std::ostringstream ss;
   (void)std::initializer_list< int >{ ( ss << args, 0 )... };
   return ss.str( );
}



} // namespace base
