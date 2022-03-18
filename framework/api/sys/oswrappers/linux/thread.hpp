#pragma once

#include <pthread.h>
#include <cstdint>



namespace carpc::os::os_linux::thread {

   using tID = pthread_t;

   const std::uint64_t convert( const tID& );

} // namespace carpc::os::os_linux::thread
