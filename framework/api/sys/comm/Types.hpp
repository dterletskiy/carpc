#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"
#include "api/sys/common/IPC.hpp"



namespace base::comm {

   namespace async
   {
      class AsyncID;
   }

   namespace service
   {
      class ServiceID;
      using ID = base::TID< ServiceID >;
      using Name = base::TName< ServiceID >;
   }

   namespace sequence
   {
      class SequenceID;
      using ID = base::TID< SequenceID >;
   }

   namespace timer
   {
      class TimerID;
      using ID = base::TID< TimerID >;
      using Name = base::TName< TimerID >;
   }

} // namespace base::service
