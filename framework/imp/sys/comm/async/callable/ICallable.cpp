#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/comm/async/callable/ICallable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ICallable"


using namespace base::async;



const bool ICallable::send( )
{
   application::IThread::tSptr p_thread = application::Process::instance( )->current_thread( );
   if( nullptr == p_thread )
   {
      SYS_ERR( "sending runnable not from application thread" );
      return false;
   }

   return p_thread->insert_event( shared_from_this( ) );
}

const bool ICallable::send_to_context( application::IThread::tWptr pw_thread )
{
   application::IThread::tSptr p_thread = pw_thread.lock( );
   if( nullptr == p_thread )
   {
      SYS_ERR( "sending runnable to not valid application thread" );
      return false;
   }

   return p_thread->insert_event( shared_from_this( ) );
}

void ICallable::process( IAsync::IConsumer* p_consumer ) const
{
   call( );
}
