#pragma once

#include "imp/experimental/base/event/TEvent.hpp"



#define DECLARE_EVENT( eventType, dataType, consumerType ) \
   using eventType      = fw::event::TCommon< dataType >::tEvent; \
   using consumerType   = fw::event::TCommon< dataType >::tConsumer;

#define DEFINE_EVENT( eventType ) \
   template< > fw::event::tClassID eventType::s_class_id = { #eventType };
