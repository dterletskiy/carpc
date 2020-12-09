#pragma once

#include "imp/experimental/base/event/Event.hpp"



namespace events {

   struct OnOffEventData
   {
      std::string message = { };
   };

   DECLARE_EVENT( OnOffEvent, OnOffEventData, IOnOffEventConsumer );

}
