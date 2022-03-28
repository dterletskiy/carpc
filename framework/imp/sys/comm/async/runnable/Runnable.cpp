#include "api/sys/oswrappers/ConditionVariable.hpp"
#include "api/sys/comm/async/runnable/Runnable.hpp"

#include "api/sys/trace/Trace.hpp"
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
            to_context.name( ).c_str( ),
            application::Context::current( ).name( ).c_str( )
         );
      return false;
   }

   return create( operation )->send( to_context, is_block );








   // Sending non blocking Async object
   if( false == is_block )
      return create( operation )->send( to_context );

   if( to_context == application::Context::current( ) )
   {
      SYS_ERR(
            "destination context '%s' = calling context '%s'",
            to_context.name( ).c_str( ),
            application::Context::current( ).name( ).c_str( )
         );
      return false;
   }

   // Sending blocking Async object
   carpc::os::ConditionVariable cond_var;

   auto operation_wrapper = [ &operation, &cond_var ]( )
   {
      if( operation )
         operation( );

      cond_var.notify( );
   };

   if( false == create( operation_wrapper )->send( to_context ) )
      return false;

   while ( true != cond_var.test( ) )
      cond_var.wait( );

   return true;
}
