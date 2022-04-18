#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"
#include "api/sys/common/Version.hpp"
#include "api/sys/common/IPC.hpp"
#include "api/sys/common/Types.hpp"



namespace carpc::comm {

   namespace async
   {
      class Async;
   }

   namespace service
   {
      class Service;
      using ID = carpc::TID< Service >;
      using Name = carpc::TName< Service >;
      using Version = carpc::TVersion< Service >;
   }

   namespace sequence
   {
      class Sequence;
      using ID = carpc::TID< Sequence >;
   }

   namespace timer
   {
      class Timer;
      using ID = carpc::TID< Timer >;
      using Name = carpc::TName< Timer >;
   }

} // namespace carpc::service
