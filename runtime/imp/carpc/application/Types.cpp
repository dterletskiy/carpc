#include "carpc/application/Process.hpp"
#include "carpc/application/IThread.hpp"
#include "carpc/application/Types.hpp"



namespace carpc::application {

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

      const ID& id( const std::string& name )
      {
         IThread::tSptr thread = Process::instance( )->thread( name );
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
