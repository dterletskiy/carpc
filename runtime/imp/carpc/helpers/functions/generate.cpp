#include <unistd.h>
#include <random>
#include <sstream>

#include "carpc/helpers/functions/generate.hpp"



namespace carpc::generate::random {

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

   // link: https://stackoverflow.com/a/60198074/12978480
   static std::random_device                 rd;
   static std::mt19937                       gen( rd( ) );
   static std::uniform_int_distribution< >   dis( 0, 15 );
   static std::uniform_int_distribution< >   dis2( 8, 11 );
   void uuid( std::string& _uuid )
   {
      std::stringstream ss;
      ss << std::hex;
      for( std::size_t i = 0; i < 8; i++ ) ss << dis( gen );
      ss << "-";
      for( std::size_t i = 0; i < 4; i++ ) ss << dis( gen );
      ss << "-4";
      for( std::size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      ss << dis2( gen );
      for( std::size_t i = 0; i < 3; i++ ) ss << dis( gen );
      ss << "-";
      for( std::size_t i = 0; i < 12; i++ ) ss << dis( gen );

      _uuid = ss.str( );
   }

}