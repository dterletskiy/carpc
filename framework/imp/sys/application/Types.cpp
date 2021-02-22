#include "api/sys/application/Process.hpp"
#include "api/sys/application/IThread.hpp"
#include "api/sys/application/Types.hpp"



namespace base::application {

   namespace process {

      ID current( )
      {
         return Process::instance( )->id( );
      }

   }

   namespace thread {

      ID current( )
      {
         ID tid{ ID::invalid( ) };
         IThread::tSptr thread = Process::instance( )->current_thread( );
         if( nullptr != thread )
            tid = thread->id( );
         return tid;
      }

   }

}
