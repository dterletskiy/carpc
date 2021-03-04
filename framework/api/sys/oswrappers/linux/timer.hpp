#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>



namespace base::os::linux::timer {

   enum class eTimerType : size_t { single = 0, continious };

   using tID = timer_t;
   using tEventHandler = void(*)( union sigval );

   bool create( tID& timer_id, int signal );
   bool create( tID& timer_id, tEventHandler callback, void* p_attributes = nullptr );
   bool remove( const tID& timer_id );
   bool start( const tID& timer_id, long int freq_nanosecs, eTimerType type = eTimerType::single );
   bool stop( const tID& timer_id );

} // namespace base::os::linux::timer



#if 0 // Examples

   using tSignalHandler = void(*)( int, siginfo_t*, void* );

   void signal_handler( int sig, siginfo_t* si, void* uc )
   {
      MSG_VRB( "Signal %d", sig );
      tID* timer_id = static_cast< tID* >( si->si_value.sival_ptr );
      MSG_DBG( "Elapsed timer with ID: %#lx", (long) *timer_id );

      // signal( sig, SIG_IGN );
   }

   void event_handler( union sigval sv )
   {
      MSG_VRB( "Event" );
      tID* timer_id = static_cast< tID* >( sv.sival_ptr );
      MSG_DBG( "Elapsed timer with ID: %#lx", (long) *timer_id );
   }

   bool set_signal_handler( int signal, tSignalHandler callback )
   {
      MSG_DBG( );

      /* Устанавливаем обработчик для сигнала таймера */
      MSG_DBG( "Setup signal handler %d", signal );
      struct sigaction sa;
      sa.sa_flags = SA_SIGINFO;
      sa.sa_sigaction = callback;
      sigemptyset( &sa.sa_mask );
      if( sigaction( signal, &sa, nullptr ) == -1 )
      {
         MSG_ERR( "sigaction" );
         return false;
      }
      return true;
   }

   void thread_loop_1( )
   {
      MSG_DBG( );

      tID timer_id;
      if( false == base::os::linux::timer::create( timer_id, SIGRTMIN ) )
         return;
      MSG_DBG( "Timer ID: %#lx", (long) timer_id );
      if( false == base::os::linux::timer::start( timer_id, 1000000000, eTimerType::continious ) )
         return;

      while( true ) { }

      base::os::linux::timer::remove( timer_id );
   }

   void thread_loop_2( )
   {
      MSG_DBG( );

      tID timer_id;
      if( false == base::os::linux::timer::create( timer_id, event_handler ) )
         return;
      MSG_DBG( "Timer ID: %#lx", (long) timer_id );
      if( false == base::os::linux::timer::start( timer_id, 1500000000, eTimerType::continious ) )
         return;

      while( true ) { }

      base::os::linux::timer::remove( timer_id );
   }



   int main( int argc, char *argv[] )
   {
      MSG_DBG( "argc = %d", argc );

      set_signal_handler( SIGRTMIN, signal_handler );

      base::os::Thread thread_1( thread_loop_1 );
      base::os::Thread thread_2( thread_loop_2 );

      sigset_t mask;
      /* Временно блокируем сигнал таймера */
      // MSG_DBG( "Blocking signal %d", SIGRTMIN );
      // sigemptyset( &mask );
      // sigaddset( &mask, SIGRTMIN );
      // if( sigprocmask( SIG_SETMASK, &mask, nullptr ) == -1 )
      // {
      //    MSG_ERR( "sigprocmask" );
      //    exit( EXIT_FAILURE );
      // }

      thread_1.run( );
      thread_2.run( );

      /* Разблокируем сигнал таймера, чтобы доставлялись уведомления таймера */
      // MSG_DBG( "Unblocking signal %d", SIGRTMIN );
      // if( sigprocmask( SIG_UNBLOCK, &mask, nullptr ) == -1 )
      // {
      //    MSG_ERR( "sigprocmask" );
      //    exit( EXIT_FAILURE );
      // }

      thread_1.join( );
      thread_2.join( );


      exit( EXIT_SUCCESS );
   }

#endif



// Issue with timer when linux creats several timers with the same ID in case of using variant with:
//    sev.sigev_notify = SIGEV_SIGNAL;
// Commit version 2.3.12
#if 0

   #include <thread>
   #include "api/sys/oswrappers/linux/signals.hpp"
   #include "api/sys/oswrappers/linux/timer.hpp"



   namespace bos = base::os::linux;



   void timer_handler( union sigval sv )
   {
      bos::timer::tID* timer_id = static_cast< timer_t* >( sv.sival_ptr );
      SYS_INF( "timer id: %#lx", (long) *timer_id );
   }

   void signal_handler( int signal, siginfo_t* si, void* uc )
   {
      bos::timer::tID* timer_id = (bos::timer::tID*)si->si_value.sival_ptr;
      SYS_VRB( "timer id: %#lx", (long)(*timer_id) );
   }

   void thread_loop_signal( )
   {
      bos::timer::tID timer_id;
      bos::timer::create( timer_id, timer_handler );
      bos::timer::start( timer_id, 3000000000, bos::timer::eTimerType::continious );
      SYS_VRB( "timer created with id: %#lx", timer_id );

      while( true );
   }


   void thread_loop_handler( )
   {
      bos::timer::tID timer_id;
      bos::signals::set_handler( SIGRTMIN, signal_handler );
      bos::timer::create( timer_id, SIGRTMIN );
      bos::timer::start( timer_id, 3000000000, bos::timer::eTimerType::continious );
      SYS_VRB( "timer created with id: %#lx", timer_id );

      while( true );
   }



   int main( int argc, char** argv, char** envp )
   {
      // bos::timer::tID timer_id_1;
      // bos::timer::create( timer_id_1, timer_handler );
      // bos::timer::start( timer_id_1, 1000000000, bos::timer::eTimerType::continious );

      // bos::timer::tID timer_id_2;
      // bos::timer::create( timer_id_2, timer_handler );
      // bos::timer::start( timer_id_2, 2000000000, bos::timer::eTimerType::continious );



      // bos::timer::tID timer_id_3;
      // bos::signals::set_handler( SIGRTMIN, signal_handler );
      // bos::timer::create( timer_id_3, SIGRTMIN );
      // bos::timer::start( timer_id_3, 3000000000, bos::timer::eTimerType::continious );

      // bos::timer::tID timer_id_4;
      // bos::signals::set_handler( SIGRTMIN, signal_handler );
      // bos::timer::create( timer_id_4, SIGRTMIN );
      // bos::timer::start( timer_id_4, 4000000000, bos::timer::eTimerType::continious );



      // auto thread_1 = std::thread( thread_loop_signal );
      // auto thread_2 = std::thread( thread_loop_signal );
      // thread_1.join( );
      // thread_2.join( );



      auto thread_3 = std::thread( thread_loop_handler );
      auto thread_4 = std::thread( thread_loop_handler );
      thread_3.join( );
      thread_4.join( );



      // while( true );



      return 0;
   }

#endif
