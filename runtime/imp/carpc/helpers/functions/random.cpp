#include <unistd.h>

#include "carpc/helpers/functions/random.hpp"



namespace carpc::random {

   void init( )
   {
      static bool initialized = false;
      if( not initialized )
      {
         srand( (unsigned)time( nullptr ) * getpid( ) );
         initialized = true;
      }
   }

   // https://stackoverflow.com/a/440240
   // https://codereview.stackexchange.com/a/29200
   std::string text( const std::size_t length )
   {
      init( );

      static const char alphanum[ ] =
         "0123456789"
         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
         "abcdefghijklmnopqrstuvwxyz";
      std::string tmp_s;
      tmp_s.reserve( length );

      for( std::size_t i = 0; i < length; ++i )
      {
         tmp_s += alphanum[ rand( ) % ( sizeof( alphanum ) - 1 ) ];
      }

      return tmp_s;
   }

   std::size_t number( const std::size_t begin, const std::size_t end )
   {
      init( );

      return begin + ( rand( ) % ( end - begin + 1 ) );
   }

}