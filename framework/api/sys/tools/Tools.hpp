#pragma once

#include "api/sys/common/Includes.hpp"



namespace base::tools::cmd {

   char* get_option( int, char**, const std::string& );
   bool is_option_exists( int , char**, const std::string&  );

   using tCmdLineMap = std::map< std::string, std::string >;
   using tCmdParamValue = std::optional< std::string >;

   void init( int argc, char** argv );
   tCmdParamValue argument( const std::string& parameter );
   const tCmdLineMap& map( );
   void print( );

} // namespace base::tools::cmd



namespace base::tools::cfg {

   using tCfgLineMap = std::map< std::string, std::string >;
   using tCfgParamValue = std::optional< std::string >;

   void init( const std::string& );
   tCfgParamValue argument( const std::string& parameter );
   const tCfgLineMap& map( );
   void print( );

} // namespace base::tools::cfg





// link: https://stackoverflow.com/a/60198074/12978480
namespace base::tools::uuid {

   void generate_random( std::string& );

} //namespace base::tools::uuid
