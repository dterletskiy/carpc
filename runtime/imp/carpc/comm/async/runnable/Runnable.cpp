#include "carpc/oswrappers/ConditionVariable.hpp"
#include "carpc/comm/async/runnable/Runnable.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Runnable"


using namespace carpc::async;



Runnable::tSptr Runnable::create( const tOperation operation )
{
   return std::shared_ptr< Runnable >( new Runnable( operation ) );
}

const bool Runnable::create_send( const tOperation operation, const application::Context& to_context, const bool is_block )
{
   if( true == is_block && to_context == application::Context::current( ) )
   {
      SYS_ERR(
            "sending blocking runnable object to destination context '%s' = calling context '%s'",
            to_context.dbg_name( ).c_str( ),
            application::Context::current( ).dbg_name( ).c_str( )
         );
      return false;
   }

   return create( operation )->send( to_context, is_block );
}
