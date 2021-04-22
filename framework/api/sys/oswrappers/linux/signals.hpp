#pragma once

#include <signal.h>
#include <vector>



namespace base::os::os_linux::signals {

   using tSignal = int;
   using tHandler = void(*)( tSignal, siginfo_t*, void* );

   void info( );

   /****************************************************************************
    *
    * Function for blocking Linux signals.
    * Parameters:
    *    - signals - list of signals to be blocked
    *
    ***************************************************************************/
   bool block( const std::vector< tSignal >& signals, const bool is_thread = false );

   /****************************************************************************
    *
    * Function for blocking Linux signals with resetting previous block state.
    * Parameters:
    *    - signals - list of signals to be blocked
    *
    ***************************************************************************/
   bool rblock( const std::vector< tSignal >& signals, const bool is_thread = false );

   /****************************************************************************
    *
    * Function for unblocking Linux signals.
    * Parameters:
    *    - signals - list of signals to be unblocked
    *
    ***************************************************************************/
   bool unblock( const std::vector< tSignal >& signals, const bool is_thread = false );

   /****************************************************************************
    *
    * Function for unblocking all Linux signals.
    *
    ***************************************************************************/
   bool unblock_all( const bool is_thread = false );

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
