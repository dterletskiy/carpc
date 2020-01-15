#pragma once

#include <string>



namespace base::os::linux {

   bool get_mac( const std::string& connection_name, std::string& connection_mac );

} // namespace base::os::linux
