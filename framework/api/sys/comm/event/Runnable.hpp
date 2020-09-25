#pragma once

#include "api/sys/comm/event/IRunnable.hpp"



namespace base {

   class Runnable : public IRunnable
   {
      public:
         Runnable( const tOperation );
         ~Runnable( ) override = default;

      public:
         static tSptr create( const tOperation );
         static const bool create_send( const tOperation );
         static const bool create_send_to_context( const tOperation, IServiceThread::tWptr );
   };



   inline
   Runnable::Runnable( const tOperation operation )
      : IRunnable( operation )
   {
   }

} // namespace base
