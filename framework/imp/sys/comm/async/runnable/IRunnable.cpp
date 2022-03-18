#include "api/sys/application/Process.hpp"
#include "api/sys/comm/async/runnable/IRunnable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IRunnable"


using namespace carpc::async;



const bool IRunnable::send( const application::Context& to_context )
{
   auto p_runnable = shared_from_this( );
   SYS_VRB( "runnable: %s", p_runnable->signature( )->name( ).c_str( ) );

   if( to_context.is_external( ) )
   {
      SYS_WRN( "runnable can't be sent as external" );
      return false;
   }
   else if( application::thread::broadcast == to_context.tid( ) )
   {
      SYS_INF( "sending broadcast runnable to all application threads" );
      bool result = true;

      application::IThread::tSptr p_thread_ipc = application::Process::instance( )->thread_ipc( );
      if( nullptr == p_thread_ipc )
      {
         SYS_ERR( "application IPC thread is not started" );
         result = false;
      }
      else
      {
         result &= p_thread_ipc->insert_event( p_runnable );
      }

      application::IThread::tSptrList thread_list = carpc::application::Process::instance( )->thread_list( );
      for( auto p_thread : thread_list )
         result &= p_thread->insert_event( p_runnable );

      return result;
   }
   else if( application::thread::local == to_context.tid( ) )
   {
      SYS_INF( "sending runnable to current application thread: %s", to_context.tid( ).name( ).c_str( ) );
      application::IThread::tSptr p_thread = carpc::application::Process::instance( )->current_thread( );
      if( nullptr == p_thread )
      {
         SYS_ERR( "sending runnable not from application thread" );
         return false;
      }

      return p_thread->insert_event( p_runnable );
   }
   else
   {
      SYS_INF( "sending runnable to %s application thread", to_context.tid( ).name( ).c_str( ) );
      application::IThread::tSptr p_thread = carpc::application::Process::instance( )->thread( to_context.tid( ) );
      if( nullptr == p_thread )
      {
         SYS_ERR( "sending runnable to unknown application thread" );
         return false;
      }

      return p_thread->insert_event( p_runnable );
   }

   return true;
}

void IRunnable::process( IAsync::IConsumer* p_consumer ) const
{
   if( nullptr == m_operation ) return;
   m_operation( );
}
