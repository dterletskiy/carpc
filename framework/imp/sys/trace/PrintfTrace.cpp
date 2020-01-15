#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/trace/PrintfTrace.hpp"



namespace base::trace::printf {

   namespace simple {

      base::os::Mutex dbg_mutex;
      void dbg_lock( )     { dbg_mutex.lock( );    }
      void dbg_unlock( )   { dbg_mutex.unlock( );  }

      char p_buffer[ 1024 ];

      timeval tv;

   }

   namespace extended {

      base::os::Mutex dbg_mutex;
      void dbg_lock( )     { dbg_mutex.lock( );    }
      void dbg_unlock( )   { dbg_mutex.unlock( );  }

      char p_buffer[ 1024 ];

      tm* time_tm;
      size_t milliseconds = 0;

   }

} // namespace base::trace::printf
