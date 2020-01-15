#include "api/sys/comm/Types.hpp"


namespace base {



const char* c_str( const eCommType comm_type )
{
   switch( comm_type )
   {
      case eCommType::ETC:    return "eCommType::ETC";
      case eCommType::ITC:    return "eCommType::ITC";
      case eCommType::IPC:    return "eCommType::IPC";
      case eCommType::NONE:   return "eCommType::NONE";
      default:                return "eCommType::UNEFINED";
   }
}


} // namespace base
