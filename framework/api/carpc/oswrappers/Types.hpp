#pragma once

#include "carpc/common/ID.hpp"
#include "carpc/common/Name.hpp"



namespace carpc::os {

   namespace futex
   {
      class FutexID;
      using ID = carpc::TID< FutexID >;
      using Name = carpc::TName< FutexID >;
   }

   namespace mutex
   {
      class MutexID;
      using ID = carpc::TID< MutexID >;
      using Name = carpc::TName< MutexID >;
   }

   namespace spinlock
   {
      class LockID;
      using ID = carpc::TID< LockID >;
      using Name = carpc::TName< LockID >;
   }

   namespace thread
   {
      class ThreadID;
      using ID = carpc::TID< ThreadID >;
      using Name = carpc::TName< ThreadID >;
   }

   namespace socket
   {
      class SocketID;
      using ID = carpc::TID< SocketID >;
      using Name = carpc::TName< SocketID >;
   }

} // namespace carpc::os
