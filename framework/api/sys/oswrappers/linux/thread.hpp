#pragma once

#include <pthread.h>
#include <cstdint>



namespace base::os::linux::thread {

   using tID = pthread_t;

   const std::uint64_t convert( const tID& );

} // namespace base::os::linux::thread
