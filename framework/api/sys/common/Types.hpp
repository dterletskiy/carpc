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
#include <fstream>
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
#include <limits>

#pragma once



namespace base {

   using ID = std::size_t;
   const ID InvalidID = std::numeric_limits< std::size_t >::max( );

} // namespace base
