#include <cstring>
#include <algorithm>
#include "carpc/oswrappers/linux/thread.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Thread"



namespace carpc::os::os_linux::thread {

   const std::uint64_t convert( const tID& tid )
   {
      std::uint64_t uint64_id = 0; // static_cast< std::uint64_t >( id );
      memcpy( &uint64_id, &tid, std::min( sizeof(uint64_id), sizeof(tid) ) );

      return uint64_id;
   }

} // namespace carpc::os::os_linux::thread
