#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/trace/WriteTrace.hpp"



namespace base::trace::write {

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

char p_buffer[ 1024 ];



} // namespace base::trace::write
