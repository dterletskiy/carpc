#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/trace/AndroidTrace.hpp"



#if OS == ANDROID

namespace base::trace::android {

   namespace simple {

      base::os::Mutex dbg_mutex;
      void dbg_lock( )     { dbg_mutex.lock( );    }
      void dbg_unlock( )   { dbg_mutex.unlock( );  }

      char p_buffer[ 1024 ];

      timeval tv;

   }

} // namespace base::trace::android

#endif // OS == ANDROID
