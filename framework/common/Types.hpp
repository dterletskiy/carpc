#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cinttypes>


#include <stdlib.h>
#include <string.h>

#include <stdio.h>  /* puts() */
#include <unistd.h> /* sleep() */

#include <sstream>
#include <type_traits>

#include <string>
#include <vector>
#include <queue>
#include <deque>
#include <list>
#include <set>
#include <map>

#include <functional>
#include <memory>
#include <optional>

#if OS == STD
   #include <thread>
#endif
#if OS == LINUX
   #include <pthread.h>
#endif
#if OS == WINDOWS
#endif

#pragma once



namespace base {



using ID             = std::uint64_t;

using IStream        = std::istream;
using OStream        = std::ostream;

using IPC_BUFFER     = char*;

#if OS == LINUX
   #define TID       pthread_t
#elif OS == STD
   #define TID       std::thread::id
#endif



} // namespace base
