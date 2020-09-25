#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/comm/event/IRunnable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "IRunnable"


using namespace base::async;



const bool IRunnable::send( )
{
   IServiceThread::tSptr p_service = ServiceProcess::instance( )->current_service( );
   if( nullptr == p_service )
   {
      SYS_ERR( "sending runnable not from service thread" );
      return false;
   }

   return p_service->insert_event( shared_from_this( ) );
}

const bool IRunnable::send_to_context( IServiceThread::tWptr pw_service )
{
   IServiceThread::tSptr p_service = pw_service.lock( );
   if( nullptr == p_service )
   {
      SYS_ERR( "sending runnable to not valid service thread" );
      return false;
   }

   return p_service->insert_event( shared_from_this( ) );
}

void IRunnable::process( IAsync::IConsumer* p_consumer ) const
{
   SYS_INF( "operation: %p", m_operation.target< tOperationPtr >( ) );
   if( nullptr == m_operation ) return;
   m_operation( );
}
