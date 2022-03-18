#include "api/sys/application/Process.hpp"
#include "api/sys/application/IThread.hpp"
#include "api/sys/application/Types.hpp"



namespace base::application {

   namespace process {

      const ID& current_id( )
      {
         return Process::instance( )->id( );
      }

   }

   namespace thread {

      const ID& current_id( )
      {
         IThread::tSptr thread = Process::instance( )->current_thread( );
         if( nullptr != thread )
            return thread->id( );

         return invalid;
      }

   }

   namespace configuration {

      const Data& current( )
      {
         return Process::instance( )->configuration( );
      }

   }

}
