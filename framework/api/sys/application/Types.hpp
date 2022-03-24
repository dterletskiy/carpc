#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"
#include "api/sys/common/Priority.hpp"
#include "api/sys/oswrappers/linux/socket.hpp"



namespace carpc::application {

   namespace component
   {
      class ComponentID;
      using ID = carpc::TID< ComponentID >;
      const ID invalid = ID::invalid;
      using Name = carpc::TName< ComponentID >;

   }

   namespace process
   {
      class ProcessID;
      using ID = carpc::TID< ProcessID >;
      const ID invalid = ID::invalid;
      const ID broadcast = ID::invalid - ID::VALUE_TYPE( 1 );
      const ID local = broadcast - ID::VALUE_TYPE( 1 );
      const ID& current_id( );
      using Name = carpc::TName< ProcessID >;

   }

   namespace thread
   {
      class ThreadID;
      using ID = carpc::TID< ThreadID >;
      const ID invalid = ID::invalid;
      const ID broadcast = ID::invalid - ID::VALUE_TYPE( 1 );
      const ID local = broadcast - ID::VALUE_TYPE( 1 );
      const ID& current_id( );
      using Name = carpc::TName< ThreadID >;

   }

   namespace configuration
   {
      struct IPC
      {
         os::os_linux::socket::configuration socket;
         std::size_t                         buffer_size;
      };
      struct Data
      {
         bool ipc = true;

         IPC ipc_sb;
         IPC ipc_app;

         std::size_t wd_timout = -1;
         const tPriority max_priority = priority( ePriority::MAX );
      };

      const Data& current( );
   }

} // namespace carpc::application
