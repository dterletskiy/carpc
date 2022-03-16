#pragma once

#include "api/sys/comm/async/event/IEvent.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TConsumer"



namespace base::async {

   template< typename _Generator >
   struct TConsumer : public IEvent::IConsumer
   {
      using tEvent = typename _Generator::Config::tEvent;

      TConsumer( ) = default;
      ~TConsumer( ) override { tEvent::clear_all_notifications( this ); }

      virtual void process_event( const tEvent& ) = 0;
   };

} // namespace base::async



#undef CLASS_ABBR
