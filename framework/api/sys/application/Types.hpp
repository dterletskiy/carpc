#pragma once

#include "api/sys/common/ID.hpp"



namespace base::application {

   class Process;
   namespace process {
      using ID = base::TID< Process >;
      const ID invalid = ID::invalid( );
      const ID broadcast = ID::invalid( ) - ID::VALUE_TYPE( 1 );
      const ID local = broadcast - ID::VALUE_TYPE( 1 );
      ID current( );
   }

   class Thread;
   namespace thread {
      using ID = base::TID< Thread >;
      const ID invalid = ID::invalid( );
      const ID broadcast = ID::invalid( ) - ID::VALUE_TYPE( 1 );
      const ID local = broadcast - ID::VALUE_TYPE( 1 );
      ID current( );
   }

} // namespace base::application
