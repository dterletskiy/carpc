#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cinttypes>


#include <stdlib.h>
#include <string.h>

#include <stdio.h>  /* puts() */
#include <unistd.h> /* sleep() */

#include <iostream>
#include <sstream>
#include <iomanip>
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

#pragma once



namespace base {

   using ID = size_t;


   struct RawBuffer
   {
      static RawBuffer create( const size_t size );
      static void destroy( RawBuffer& buffer );

      bool alloc( const size_t size );
      void free( );
      void dump( ) const;

      void* ptr = nullptr;
      size_t size = 0;
   };


} // namespace base
