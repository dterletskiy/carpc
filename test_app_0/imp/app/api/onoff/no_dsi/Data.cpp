// Application
#include "Data.hpp"



namespace api::onoff::no_dsi {




RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const size_t _delay  )
   : BaseData( )
   , state( _state )
   , delay( _delay )
{
}

ResponseTriggerStateData::ResponseTriggerStateData( const bool _result )
   : BaseData( )
   , result( _result )
{
}

NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
   : BaseData( )
   , state( _state )
{
}

OnOffEventData::OnOffEventData( tBaseDataPtr _ptr )
   : ptr( _ptr )
{
}



} // namespace api::onoff::no_dsi



INIT_EVENT_RR( api::onoff::no_dsi::OnOffEvent );
