#include "api/sys/trace/TraceLinux.hpp"



namespace base::trace::linux {

   thread_local tm* time_tm;
   thread_local std::size_t milliseconds = 0;

} // namespace base::trace::linux
