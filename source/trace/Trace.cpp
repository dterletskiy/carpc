#include "oswrappers/Mutex.hpp"
#include "Trace.hpp"


namespace base::trace {



base::os::Mutex dbg_mutex;
void dbg_lock( )
{
   dbg_mutex.lock( );
}
void dbg_unlock( )
{
   dbg_mutex.unlock( );
}

pid_t pid = 0;
pthread_t tid = 0;

tm* time_tm;
size_t milliseconds = 0;
void sys_time( )
{
   timeval tv;
   gettimeofday( &tv, NULL );
   time_tm = localtime( &tv.tv_sec );
   milliseconds = tv.tv_usec;
}



} // namespace base::trace
