#pragma once

#include "api/sys/comm/async/runnable/IRunnable.hpp"



namespace carpc::async {

   class Runnable : public IRunnable
   {
      private:
         Runnable( const tOperation );

      public:
         ~Runnable( ) override = default;
         static tSptr create( const tOperation );
         static const bool create_send( const tOperation, const application::Context& to_context = application::Context::internal_local );
   };



   inline
   Runnable::Runnable( const tOperation operation )
      : IRunnable( operation )
   {
   }

} // namespace carpc::async
