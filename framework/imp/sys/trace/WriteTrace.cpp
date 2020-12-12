#include "api/sys/trace/WriteTrace.hpp"



namespace base::trace {

   thread_local tm* time_tm;
   thread_local std::size_t milliseconds = 0;

} // namespace base::trace
