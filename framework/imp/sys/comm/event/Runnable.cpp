#include "api/sys/application/Process.hpp"
#include "api/sys/comm/event/Runnable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Runnable"


using namespace base::async;



Runnable::tSptr Runnable::create( const tOperation operation )
{
   return std::make_shared< Runnable >( operation );
}

const bool Runnable::create_send( const tOperation operation )
{
   return create( operation )->send( );
}

const bool Runnable::create_send_to_context( const tOperation operation, application::IThread::tWptr pw_thread )
{
   return create( operation )->send_to_context( pw_thread );
}
