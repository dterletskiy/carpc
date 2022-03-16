#pragma once

#include "api/sys/common/ID.hpp"
#include "api/sys/common/Name.hpp"



namespace base::application {

   namespace component
   {
      class ComponentID;
      using ID = base::TID< ComponentID >;
      const ID invalid = ID::invalid;
      using Name = base::TName< ComponentID >;

   }

   namespace process
   {
      class ProcessID;
      using ID = base::TID< ProcessID >;
      const ID invalid = ID::invalid;
      const ID broadcast = ID::invalid - ID::VALUE_TYPE( 1 );
      const ID local = broadcast - ID::VALUE_TYPE( 1 );
      ID current( );
      using Name = base::TName< ProcessID >;

   }

   namespace thread
   {
      class ThreadID;
      using ID = base::TID< ThreadID >;
      const ID invalid = ID::invalid;
      const ID broadcast = ID::invalid - ID::VALUE_TYPE( 1 );
      const ID local = broadcast - ID::VALUE_TYPE( 1 );
      ID current( );
      using Name = base::TName< ThreadID >;

   }

} // namespace base::application
