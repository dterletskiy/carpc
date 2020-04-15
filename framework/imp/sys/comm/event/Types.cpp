#include "api/sys/comm/event/Types.hpp"


namespace base {



const char* c_str( const eAsyncType comm_type )
{
   switch( comm_type )
   {
      case eAsyncType::EVENT:       return "eAsyncType::EVENT";
      case eAsyncType::RUNNABLE:    return "eAsyncType::RUNNABLE";
      default:                      return "eAsyncType::UNEFINED";
   }
}

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
