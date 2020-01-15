#pragma once

// Framework
#include "api/sys/common/Helpers.hpp"
#include "api/sys/comm/Event.hpp"



namespace api::onoff {



DEFINE_ENUM_EX( eOnOff, size_t, RequestTriggerState, ResponseTriggerState, NotificationCurrentState );
struct OnOffEventData
{
};



} // namespace api::onoff



DECLARE_DSI_EVENT( OnOffService, OnOffEvent, api::onoff::OnOffEventData, api::onoff::eOnOff );
