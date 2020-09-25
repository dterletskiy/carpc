#include "api/sys/comm/event/Types.hpp"


namespace base {

   const char* c_str( const eAsyncType comm_type )
   {
      switch( comm_type )
      {
         case eAsyncType::EVENT:       return "base::eAsyncType::EVENT";
         case eAsyncType::RUNNABLE:    return "base::eAsyncType::RUNNABLE";
         default:                      return "base::eAsyncType::UNEFINED";
      }
   }

   const char* c_str( const eCommType comm_type )
   {
      switch( comm_type )
      {
         case eCommType::ETC:    return "base::eCommType::ETC";
         case eCommType::ITC:    return "base::eCommType::ITC";
         case eCommType::IPC:    return "base::eCommType::IPC";
         case eCommType::NONE:   return "base::eCommType::NONE";
         default:                return "base::eCommType::UNEFINED";
      }
   }

}
