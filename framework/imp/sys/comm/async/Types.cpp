#include "api/sys/comm/async/Types.hpp"


namespace base::async {

   const char* c_str( const eAsyncType async_type )
   {
      switch( async_type )
      {
         case eAsyncType::EVENT:       return "base::eAsyncType::EVENT";
         case eAsyncType::RUNNABLE:    return "base::eAsyncType::RUNNABLE";
         case eAsyncType::CALLABLE:    return "base::eAsyncType::CALLABLE";
         default:                      return "base::eAsyncType::UNEFINED";
      }
   }

}
