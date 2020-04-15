#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   using tAsyncTypeID = std::string;

   enum class eAsyncType : size_t { EVENT, RUNNABLE, UNDEFINED };
   const char* c_str( const eAsyncType );

   enum class eCommType : size_t { IPC, ITC, ETC, NONE };
   const char* c_str( const eCommType );


} // namespace base
