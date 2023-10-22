#pragma once

#include <cstdlib>
#include <string>



namespace carpc::random {

   void init( );
   std::string text( const std::size_t length );
   std::size_t number( const std::size_t begin, const std::size_t end );

}
