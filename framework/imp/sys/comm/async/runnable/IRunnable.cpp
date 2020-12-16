#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/application/Process.hpp"
#include "api/sys/comm/async/runnable/IRunnable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IRunnable"


using namespace base::async;



const bool IRunnable::send( )
{
   application::IThread::tSptr p_thread = application::Process::instance( )->current_thread( );
   if( nullptr == p_thread )
   {
      SYS_ERR( "sending runnable not from application thread" );
      return false;
   }

   return p_thread->insert_event( shared_from_this( ) );
}

const bool IRunnable::send_to_context( application::IThread::tWptr pw_thread )
{
   application::IThread::tSptr p_thread = pw_thread.lock( );
   if( nullptr == p_thread )
   {
      SYS_ERR( "sending runnable to not valid application thread" );
      return false;
   }

   return p_thread->insert_event( shared_from_this( ) );
}

void IRunnable::process( IAsync::IConsumer* p_consumer ) const
{
   SYS_INF( "operation: %p", m_operation.target< tOperationPtr >( ) );
   if( nullptr == m_operation ) return;
   m_operation( );
}
