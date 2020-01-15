#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>



namespace base::os {



enum class eTimerType : size_t
{
   single = 0, // single shot
   continious
};

using TimerID = timer_t;
using tSignalHandler = void(*)( int, siginfo_t*, void* );
using tEventHandler = void(*)( union sigval );

bool create_timer( timer_t& timer_id, int signal );
bool create_timer( timer_t& timer_id, tEventHandler callback );
bool delete_timer( const timer_t& timer_id );
bool start_timer( const timer_t& timer_id, long int freq_nanosecs, eTimerType type = eTimerType::single );



} // namespace base::os



#if 0 // Examples

void signal_handler( int sig, siginfo_t* si, void* uc )
{
   DBG_TRC( "Signal %d", sig );
   timer_t* timer_id = static_cast< timer_t* >( si->si_value.sival_ptr );
   DBG_MSG( "Elapsed timer with ID: %#lx", (long) *timer_id );

   // signal( sig, SIG_IGN );
}

void event_handler( union sigval sv )
{
   DBG_TRC( "Event" );
   timer_t* timer_id = static_cast< timer_t* >( sv.sival_ptr );
   DBG_MSG( "Elapsed timer with ID: %#lx", (long) *timer_id );
}

bool set_signal_handler( int signal, tSignalHandler callback )
{
   DBG_MSG( );

   /* Устанавливаем обработчик для сигнала таймера */
   DBG_MSG( "Setup signal handler %d", signal );
   struct sigaction sa;
   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = callback;
   sigemptyset( &sa.sa_mask );
   if( sigaction( signal, &sa, nullptr ) == -1 )
   {
      DBG_ERR( "sigaction" );
      return false;
   }
   return true;
}

void thread_loop_1( )
{
   DBG_MSG( );

   timer_t timer_id;
   if( false == create_timer( timer_id, SIGRTMIN ) )
      return;
   DBG_MSG( "Timer ID: %#lx", (long) timer_id );
   if( false == start_timer( timer_id, 1000000000, eTimerType::continious ) )
      return;

   while( true ) { }

   delete_timer( timer_id );
}

void thread_loop_2( )
{
   DBG_MSG( );

   timer_t timer_id;
   if( false == create_timer( timer_id, event_handler ) )
      return;
   DBG_MSG( "Timer ID: %#lx", (long) timer_id );
   if( false == start_timer( timer_id, 1500000000, eTimerType::continious ) )
      return;

   while( true ) { }

   delete_timer( timer_id );
}



int main( int argc, char *argv[] )
{
   DBG_MSG( "argc = %d", argc );

   set_signal_handler( SIGRTMIN, signal_handler );

   base::os::Thread thread_1( thread_loop_1 );
   base::os::Thread thread_2( thread_loop_2 );

   sigset_t mask;
   /* Временно блокируем сигнал таймера */
   // DBG_MSG( "Blocking signal %d", SIGRTMIN );
   // sigemptyset( &mask );
   // sigaddset( &mask, SIGRTMIN );
   // if( sigprocmask( SIG_SETMASK, &mask, nullptr ) == -1 )
   // {
   //    DBG_ERR( "sigprocmask" );
   //    exit( EXIT_FAILURE );
   // }

   thread_1.run( );
   thread_2.run( );

   /* Разблокируем сигнал таймера, чтобы доставлялись уведомления таймера */
   // DBG_MSG( "Unblocking signal %d", SIGRTMIN );
   // if( sigprocmask( SIG_UNBLOCK, &mask, nullptr ) == -1 )
   // {
   //    DBG_ERR( "sigprocmask" );
   //    exit( EXIT_FAILURE );
   // }

   thread_1.join( );
   thread_2.join( );


   exit( EXIT_SUCCESS );
}

#endif
