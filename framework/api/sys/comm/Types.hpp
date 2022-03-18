#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"
#include "api/sys/common/IPC.hpp"



namespace carpc::comm {

   namespace async
   {
      class AsyncID;
   }

   namespace service
   {
      class ServiceID;
      using ID = carpc::TID< ServiceID >;
      using Name = carpc::TName< ServiceID >;
   }

   namespace sequence
   {
      class SequenceID;
      using ID = carpc::TID< SequenceID >;
   }

   namespace timer
   {
      class TimerID;
      using ID = carpc::TID< TimerID >;
      using Name = carpc::TName< TimerID >;
   }

} // namespace carpc::service
