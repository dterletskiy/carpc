#include <sys/syscall.h>
#include "carpc/oswrappers/linux/signals.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "signals"



namespace carpc::os::os_linux::signals {

   thread_local int error = 0;



   bool sigwait( const sigset_t* const set, tSignal* const signo )
   {
      int result = ::sigwait( set, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "sigwait(%p, %p): error %d", set, signo, error );
         return false;
      }

      SYS_VRB( "sigwait(%p, %p)", set, signo );
      return true;
   }

   bool kill( const pid_t pid, const tSignal signo )
   {
      int result = ::kill( pid, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "kill(%d, %d): error %d", pid, signo, error );
         return false;
      }

      SYS_VRB( "kill(%p, %p)", pid, signo );
      return true;
   }

   bool tkill( const pid_t tid, const tSignal signo )
   {
      int result = ::syscall( SYS_tkill, tid, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "tkill(%d, %d): error %d", tid, signo, error );
         return false;
      }

      SYS_VRB( "tkill(%d, %d)", tid, signo );
      return true;
   }

   bool tgkill( const pid_t tgid, const pid_t tid, const tSignal signo )
   {
      int result = ::syscall( SYS_tgkill, tgid, tid, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "tgkill(%d, %d, %d): error %d", tgid, tid, signo, error );
         return false;
      }

      SYS_VRB( "tgkill(%d, %d, %d)", tgid, tid, signo );
      return true;
   }

   int pthread_kill( const pthread_t thread, const int signo )
   {
      int result = ::pthread_kill( thread, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "pthread_kill(%#lx, %d): error %d", thread, signo, error );
         return false;
      }

      SYS_VRB( "pthread_kill(%#lx, %d)", thread, signo );
      return true;
   }

   bool sigprocmask( const int how, const sigset_t* const set, sigset_t* const oldset )
   {
      int result = ::sigprocmask( how, set, oldset );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "sigprocmask(%d, %p, %p): error %d", how, set, oldset, error );
         return false;
      }

      SYS_VRB( "sigprocmask(%d, %p, %p)", how, set, oldset );
      return true;
   }

   bool pthread_sigmask( const int how, const sigset_t* const set, sigset_t* const oldset )
   {
      int result = ::pthread_sigmask( how, set, oldset );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "pthread_sigmask(%d, %p, %p): error %d", how, set, oldset, error );
         return false;
      }

      SYS_VRB( "pthread_sigmask(%d, %p, %p)", how, set, oldset );
      return true;
   }

   bool sigismember( const sigset_t* const set, const tSignal signo )
   {
      int result = ::sigismember( set, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "sigismember(%p, %d): error %d", set, signo, error );
         return false;
      }

      SYS_VRB( "sigismember(%p, %d): result %d", set, signo, result );
      return 1 == result;
   }

   bool sigaddset( sigset_t* const set, const tSignal signo )
   {
      int result = ::sigaddset( set, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "sigaddset(%p, %d): error %d", set, signo, error );
         return false;
      }

      SYS_VRB( "sigaddset(%p, %d)", set, signo );
      return true;
   }

   bool sigdelset( sigset_t* const set, const tSignal signo )
   {
      int result = ::sigdelset( set, signo );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "sigdelset(%p, %d): error %d", set, signo, error );
         return false;
      }

      SYS_VRB( "sigdelset(%p, %d)", set, signo );
      return true;
   }

   bool sigfillset( sigset_t* const set )
   {
      int result = ::sigfillset( set );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "sigfillset(%p): error %d", set, error );
         return false;
      }

      SYS_VRB( "sigfillset(%p)", set );
      return true;
   }

   bool sigemptyset( sigset_t* const set )
   {
      int result = ::sigemptyset( set );
      if( -1 == result )
      {
         error = errno;
         SYS_ERR( "sigemptyset(%p): error %d", set, error );
         return false;
      }

      SYS_VRB( "sigemptyset(%p)", set );
      return true;
   }



}



namespace carpc::os::os_linux::signals {

   void info( )
   {
      SYS_DBG( "SIGRTMIN = %d", SIGRTMIN );
      SYS_DBG( "SIGRTMAX = %d", SIGRTMAX );

      sigset_t sigset;
      ::sigprocmask( 0, nullptr, &sigset );

      SYS_DBG( "Blocked signals: " );
      for( int signo = SIGRTMIN; signo <= SIGRTMAX; ++signo )
      {
         if( ::sigismember( &sigset, signo ) )
         {
            SYS_DBG( "%d", signo );
         }
      }
   }

   bool block( const std::vector< tSignal >& signals )
   {
      if( signals.empty( ) )
      {
         SYS_ERR( "empty siglas list" );
         return false;
      }

      sigset_t mask, old_mask;
      ::sigemptyset( &mask );
      for( tSignal signal: signals )
         ::sigaddset( &mask, signal );

      if( -1 == ::pthread_sigmask( SIG_BLOCK, &mask, &old_mask ) )
      {
         int last_errno = errno;
         SYS_ERR( "pthread_sigmask( ) error: %d", last_errno );
         return false;
      }

      return true;
   }

   bool rblock( const std::vector< tSignal >& signals )
   {
      if( signals.empty( ) )
      {
         SYS_ERR( "empty siglas list" );
         return false;
      }

      sigset_t mask, old_mask;
      ::sigemptyset( &mask );
      for( tSignal signal: signals )
         ::sigaddset( &mask, signal );

      if( -1 == ::pthread_sigmask( SIG_SETMASK, &mask, &old_mask ) )
      {
         int last_errno = errno;
         SYS_ERR( "pthread_sigmask( ) error: %d", last_errno );
         return false;
      }

      return true;
   }

   bool unblock( const std::vector< tSignal >& signals )
   {
      if( signals.empty( ) )
      {
         SYS_ERR( "empty siglas list" );
         return false;
      }

      sigset_t mask, old_mask;
      ::sigemptyset( &mask );
      for( tSignal signal: signals )
         ::sigaddset( &mask, signal );

      if( -1 == ::pthread_sigmask( SIG_UNBLOCK, &mask, &old_mask ) )
      {
         int last_errno = errno;
         SYS_ERR( "pthread_sigmask( ) error: %d", last_errno );
         return false;
      }

      return true;
   }

   bool unblock_all( )
   {
      sigset_t mask, old_mask;
      ::sigemptyset( &mask );

      if( -1 == ::pthread_sigmask( SIG_SETMASK, &mask, &old_mask ) )
      {
         int last_errno = errno;
         SYS_ERR( "pthread_sigmask( ) error: %d", last_errno );
         return false;
      }

      return true;
   }

   bool set_handler( const tSignal signal, tHandler handler )
   {
      if( nullptr == handler )
      {
         SYS_ERR( "handler is null poiner" );
         return false;
      }

      struct sigaction sa, old_sa;
      sa.sa_flags = SA_SIGINFO;
      sa.sa_sigaction = handler;
      ::sigemptyset( &sa.sa_mask );
      if( -1 == ::sigaction( signal, &sa, &old_sa ) )
      {
         int last_errno = errno;
         SYS_ERR( "sigaction error: %d", last_errno );
         return false;
      }
      return true;
   }

   bool reset_handler( const tSignal signal )
   {
      struct sigaction sa, old_sa;
      sa.sa_handler = SIG_DFL;
      ::sigemptyset( &sa.sa_mask );
      if( -1 == ::sigaction( signal, &sa, &old_sa ) )
      {
         int last_errno = errno;
         SYS_ERR( "sigaction error: %d", last_errno );
         return false;
      }
      return true;
   }

   bool ignore( const tSignal signal )
   {
      struct sigaction sa, old_sa;
      sa.sa_handler = SIG_IGN;
      ::sigemptyset( &sa.sa_mask );
      if( -1 == ::sigaction( signal, &sa, &old_sa ) )
      {
         int last_errno = errno;
         SYS_ERR( "sigaction error: %d", last_errno );
         return false;
      }
      return true;
   }

   bool wait( const std::vector< tSignal >& signals, tSignal* p_signal )
   {
      sigset_t mask;
      ::sigemptyset( &mask );
      for( tSignal signal: signals )
         ::sigaddset( &mask, signal );

      int result = ::sigwait( &mask, p_signal );
      if( 0 != result )
      {
         int last_errno = errno;
         SYS_ERR( "wait error: %d(%d)", last_errno, result );
         return false;
      }

      SYS_VRB( "%d", *p_signal );
      return true;
   }


} // namespace carpc::os::os_linux::signals
