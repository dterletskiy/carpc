#pragma once

#include "carpc/runtime/comm/async/event/IEvent.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "TConsumer"



namespace carpc::async {

   template< typename _Generator >
   struct TConsumer : public IEvent::IConsumer
   {
      using tEvent = typename _Generator::Config::tEvent;

      TConsumer( ) = default;
      ~TConsumer( ) override { tEvent::clear_all_notifications( this ); }

      virtual void process_event( const tEvent& ) = 0;
   };

} // namespace carpc::async



#undef CLASS_ABBR
