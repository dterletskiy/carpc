#include <map>
#include "api/sys/oswrappers/linux/signals.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "signals"



namespace base::os::linux::signals {

   typedef int ( *fpSignalMask ) ( int, const sigset_t*, sigset_t* );
   std::map< bool, std::pair< fpSignalMask, const char* > > map = {
      { false, { sigprocmask, "sigprocmask" } },
      { true, { pthread_sigmask, "pthread_sigmask" } }
   };

   bool block( const std::vector< tSignal >& signals, const bool is_thread )
   {
      if( signals.empty( ) )
      {
         SYS_ERR( "empty siglas list" );
         return false;
      }

      sigset_t mask, old_mask;
      sigemptyset( &mask );
      for( tSignal signal: signals )
         sigaddset( &mask, signal );

      if( ( map.find( is_thread )->second.first )( SIG_BLOCK, &mask, &old_mask ) == -1 )
      {
         int last_errno = errno;
         SYS_ERR( "%s error: %d", map.find( is_thread )->second.second, last_errno );
         return false;
      }

      SYS_VRB( "%s", map.find( is_thread )->second.second );
      return true;
   }

   bool rblock( const std::vector< tSignal >& signals, const bool is_thread )
   {
      if( signals.empty( ) )
      {
         SYS_ERR( "empty siglas list" );
         return false;
      }

      sigset_t mask, old_mask;
      sigemptyset( &mask );
      for( tSignal signal: signals )
         sigaddset( &mask, signal );

      if( ( map.find( is_thread )->second.first )( SIG_SETMASK, &mask, &old_mask ) == -1 )
      {
         int last_errno = errno;
         SYS_ERR( "%s error: %d", map.find( is_thread )->second.second, last_errno );
         return false;
      }

      SYS_VRB( "%s", map.find( is_thread )->second.second );
      return true;
   }

   bool unblock( const std::vector< tSignal >& signals, const bool is_thread )
   {
      if( signals.empty( ) )
      {
         SYS_ERR( "empty siglas list" );
         return false;
      }

      sigset_t mask, old_mask;
      sigemptyset( &mask );
      for( tSignal signal: signals )
         sigaddset( &mask, signal );

      if( -1 == ( map.find( is_thread )->second.first )( SIG_UNBLOCK, &mask, &old_mask ) )
      {
         int last_errno = errno;
         SYS_ERR( "%s error: %d", map.find( is_thread )->second.second, last_errno );
         return false;
      }

      SYS_VRB( "%s", map.find( is_thread )->second.second );
      return true;
   }

   bool unblock_all( const bool is_thread )
   {
      sigset_t mask, old_mask;
      sigemptyset( &mask );

      if( ( map.find( is_thread )->second.first )( SIG_SETMASK, &mask, &old_mask ) == -1 )
      {
         int last_errno = errno;
         SYS_ERR( "%s error: %d", map.find( is_thread )->second.second, last_errno );
         return false;
      }

      SYS_VRB( "%s", map.find( is_thread )->second.second );
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
      sigemptyset( &sa.sa_mask );
      if( sigaction( signal, &sa, &old_sa ) == -1 )
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
      sigemptyset( &sa.sa_mask );
      if( sigaction( signal, &sa, &old_sa ) == -1 )
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
      sigemptyset( &sa.sa_mask );
      if( sigaction( signal, &sa, &old_sa ) == -1 )
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
      sigemptyset( &mask );
      for( tSignal signal: signals )
         sigaddset( &mask, signal );

      int result = sigwait( &mask, p_signal );
      if( 0 != result )
      {
         int last_errno = errno;
         SYS_ERR( "wait error: %d(%d)", last_errno, result );
         return false;
      }

      SYS_VRB( "%d", *p_signal );
      return true;
   }


} // namespace base::os::linux::signals
