#include <chrono>
#include <functional>
#include <thread>
#include <limits>
#include "carpc/comm/async/runnable/Runnable.hpp"
#include "carpc/application/Process.hpp"
#include "carpc/comm/timer/Timer.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Timer"



namespace carpc::timer {

   carpc::comm::timer::ID start( const std::size_t milliseconds, const std::size_t count, tCallback callback, const bool asynchronous )
   {
      application::Context context = application::Context::current( );
      if( application::Context::invalid == context )
      {
         SYS_ERR( "application::Thread has not been found. Creating timer not in application thread" );
         return 0;
      }

      const carpc::comm::timer::ID id = carpc::comm::timer::ID::generate( );
      auto on_timer = [=]( ){ callback( id ); };

      if( asynchronous )
      {
         std::thread(
            [=]( )
            {
               for( std::size_t ticks = 0; ticks < count; ++ticks )
               {
                  std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
                  carpc::async::Runnable::create_send( on_timer, context );
               }
            }
         ).detach( );
      }
      else
      {
         for( std::size_t ticks = 0; ticks < count; ++ticks )
         {
            std::this_thread::sleep_for( std::chrono::milliseconds( milliseconds ) );
            carpc::async::Runnable::create_send( on_timer, context );
         }
      }

      return id;
   }

} // namespace carpc::timer
