#pragma once

#include "api/sys/common/Types.hpp"



namespace base {

   using EventTypeID = std::string;
   using NoServiceType = void;
   using NoIdInfoType = void;
   using SequenceID = size_t;
   extern const SequenceID InvalidSequenceID;

   enum class eCommType : size_t { IPC, ITC, ETC, NONE };
   const char* c_str( const eCommType );

   enum class eEventType : size_t { SIMPLE, SIMPLE_ID, RR, RUNNABLE, UNDEFINED };
   const char* c_str( const eEventType );


} // namespace base
