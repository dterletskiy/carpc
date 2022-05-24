#include "carpc/application/Process.hpp"
#include "carpc/comm/async/callable/ICallable.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "ICallable"


using namespace carpc::async;



const bool ICallable::send( const application::Context& to_context )
{
   auto p_callable = shared_from_this( );
   SYS_VRB( "callable: %s", p_callable->signature( )->name( ).c_str( ) );

   if( to_context.is_external( ) )
   {
      SYS_WRN( "callable can't be sent as external" );
      return false;
   }
   if( application::thread::broadcast == to_context.tid( ) )
   {
      SYS_INF( "sending broadcast callable to all application threads" );
      bool result = true;

      application::IThread::tSptr p_thread_ipc = application::Process::instance( )->thread_ipc( );
      if( nullptr == p_thread_ipc )
      {
         SYS_ERR( "application IPC thread is not started" );
         result = false;
      }
      else
      {
         result &= p_thread_ipc->insert_event( p_callable );
      }

      application::IThread::tSptrList thread_list = carpc::application::Process::instance( )->thread_list( );
      for( auto p_thread : thread_list )
         result &= p_thread->insert_event( p_callable );

      return result;
   }
   else if( application::thread::local == to_context.tid( ) )
   {
      SYS_INF( "sending callable to current application thread: %s", to_context.tid( ).name( ).c_str( ) );
      application::IThread::tSptr p_thread = carpc::application::Process::instance( )->current_thread( );
      if( nullptr == p_thread )
      {
         SYS_ERR( "sending callable not from application thread" );
         return false;
      }

      return p_thread->insert_event( p_callable );
   }
   else
   {
      SYS_INF( "sending callable to %s application thread", to_context.tid( ).name( ).c_str( ) );
      application::IThread::tSptr p_thread = carpc::application::Process::instance( )->thread( to_context.tid( ) );
      if( nullptr == p_thread )
      {
         SYS_ERR( "sending callable to unknown application thread" );
         return false;
      }

      return p_thread->insert_event( p_callable );
   }

   return true;
}

void ICallable::process( IAsync::IConsumer* p_consumer ) const
{
   call( );
}
