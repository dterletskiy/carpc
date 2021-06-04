#pragma once

#include <signal.h>
#include <vector>



namespace base::os::os_linux::signals {

   extern thread_local int error;
   using tSignal = int;
   using tHandler = void(*)( tSignal, siginfo_t*, void* );

   bool sigwait( const sigset_t* const set, tSignal* const signo );
   bool kill( const pid_t pid, const tSignal signo );
   bool tkill( const pid_t tid, const tSignal signo );
   bool tgkill( const pid_t tgid, const pid_t tid, const tSignal signo );
   int pthread_kill( const pthread_t thread, const int signo );

   bool sigprocmask( const int how, const sigset_t* const set, sigset_t* const oldset );
   bool pthread_sigmask( const int how, const sigset_t* const set, sigset_t* const oldset );

   bool sigismember( const sigset_t* const set, const tSignal signo );
   bool sigaddset( sigset_t* const set, const tSignal signo );
   bool sigdelset( sigset_t* const set, const tSignal signo );
   bool sigfillset( sigset_t* const set );
   bool sigemptyset( sigset_t* const set );

}



namespace base::os::os_linux::signals {

   void info( );

   /****************************************************************************
    *
    * Function for blocking Linux signals.
    * Parameters:
    *    - signals - list of signals to be blocked
    *
    ***************************************************************************/
   bool block( const std::vector< tSignal >& signals );

   /****************************************************************************
    *
    * Function for blocking Linux signals with resetting previous block state.
    * Parameters:
    *    - signals - list of signals to be blocked
    *
    ***************************************************************************/
   bool rblock( const std::vector< tSignal >& signals );

   /****************************************************************************
    *
    * Function for unblocking Linux signals.
    * Parameters:
    *    - signals - list of signals to be unblocked
    *
    ***************************************************************************/
   bool unblock( const std::vector< tSignal >& signals );

   /****************************************************************************
    *
    * Function for unblocking all Linux signals.
    *
    ***************************************************************************/
   bool unblock_all( );

   /****************************************************************************
    *
    * Function to set Linux signal handler.
    * Parameters:
    *    - signal - signal to handle
    *    - handler - pointer to signal handler
    *
    ***************************************************************************/
   bool set_handler( const tSignal signal, tHandler handler );

   /****************************************************************************
    *
    * Function to reset Linux signal to default handler.
    * Parameters:
    *    - signal - signal to reset handler
    *
    ***************************************************************************/
   bool reset_handler( const tSignal signal );

   /****************************************************************************
    *
    * Function to ignor Linux signal.
    * Parameters:
    *    - signal - signal to be ignored
    *
    ***************************************************************************/
   bool ignore( const tSignal signal );

   /****************************************************************************
    *
    * Function to wait Linux signals.
    * Parameters:
    *    - signals - signal list to be wait
    *    - p_signal - pointer to variable where received signal will be stored
    *
    ***************************************************************************/
   bool wait( const std::vector< tSignal >& signals, tSignal* p_signal = nullptr );

} // namespace base::os::os_linux::signals
