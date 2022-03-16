#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"



namespace base::os {

   namespace futex
   {
      class FutexID;
      using ID = base::TID< FutexID >;
      using Name = base::TName< FutexID >;
   }

   namespace mutex
   {
      class MutexID;
      using ID = base::TID< MutexID >;
      using Name = base::TName< MutexID >;
   }

   namespace spinlock
   {
      class LockID;
      using ID = base::TID< LockID >;
      using Name = base::TName< LockID >;
   }

   namespace thread
   {
      class ThreadID;
      using ID = base::TID< ThreadID >;
      using Name = base::TName< ThreadID >;
   }

   namespace socket
   {
      class SocketID;
      using ID = base::TID< SocketID >;
      using Name = base::TName< SocketID >;
   }

} // namespace base::os
