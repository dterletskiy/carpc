#pragma once

#include "api/sys/comm/async/runnable/IRunnable.hpp"



namespace base::async {

   class Runnable : public IRunnable
   {
      public:
         Runnable( const tOperation );
         ~Runnable( ) override = default;

      public:
         static tSptr create( const tOperation );
         static const bool create_send( const tOperation, const application::Context& to_context = application::Context::internal_local );
   };



   inline
   Runnable::Runnable( const tOperation operation )
      : IRunnable( operation )
   {
   }

} // namespace base::async
