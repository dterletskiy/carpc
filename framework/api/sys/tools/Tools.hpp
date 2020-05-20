#pragma once

#include "api/sys/common/Types.hpp"



namespace base::tools::id {

   const ID generate( const char* const name_space = "default" );

} // namespace base::tools::id



namespace base::tools::cmd {

   char* get_option( int, char**, const std::string& );
   bool is_option_exists( int , char**, const std::string&  );

   using tCmdParamValue = std::optional< std::string >;

   void init( int argc, char** argv );
   tCmdParamValue argument( const std::string& parameter );

} // namespace base::tools::cmd





// link: https://stackoverflow.com/a/60198074/12978480
namespace base::tools::uuid {

   void generate_random( std::string& );

} //namespace base::tools::uuid
