#pragma once

#include "api/sys/common/Types.hpp"



#define TO_STRING( VALUE ) #VALUE

namespace base::tools {

   const ID generate_id( const char* const name_space = "default" );

   char* get_cmd_option( int, char**, const std::string& );
   bool is_cmd_option_exists( int , char**, const std::string&  );

} // namespace base::tools
