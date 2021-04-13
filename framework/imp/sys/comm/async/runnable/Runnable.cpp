#include "api/sys/comm/async/runnable/Runnable.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "Runnable"


using namespace base::async;



Runnable::tSptr Runnable::create( const tOperation operation )
{
   return std::make_shared< Runnable >( operation );
}

const bool Runnable::create_send( const tOperation operation, const application::Context& to_context )
{
   return create( operation )->send( to_context );
}
