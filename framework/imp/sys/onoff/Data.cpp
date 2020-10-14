#include "api/sys/onoff/Data.hpp"



namespace base::onoff {

   const base::async::tAsyncTypeID interface_type_id = data::OnOffEvent::Signature::build_type_id( );

   const std::vector< base::service::RequestResponseIDs< eOnOff > > s_rr = {
      { eOnOff::RequestTriggerState, eOnOff::RequestTriggerStateBusy, eOnOff::ResponseTriggerState },
      { eOnOff::RequestStart, eOnOff::RequestStartBusy, eOnOff::Undefined }
   };

   const std::vector< base::service::NotificationIDs< eOnOff > > s_n = {
      { eOnOff::SubscribeCurrentState, eOnOff::UnsubscribeCurrentState, eOnOff::NotificationCurrentState }
   };

} // namespace base::onoff



namespace base::onoff::ipc {

   const std::vector< base::service::RequestResponseIDs< BaseTypes::tEventID > >& BaseTypes::RR = s_rr;
   const std::vector< base::service::NotificationIDs< BaseTypes::tEventID > >& BaseTypes::N = s_n;



   tBaseDataPtr BaseData::create( base::dsi::tByteStream& stream )
   {
      eOnOff id = eOnOff::Undefined;
      if( false == stream.pop( id ) )
         return nullptr;

      tBaseDataPtr ptr = nullptr;
      switch( id )
      {
         case eOnOff::RequestTriggerState:         ptr = std::make_shared< RequestTriggerStateData >( );       break;
         case eOnOff::ResponseTriggerState:        ptr = std::make_shared< ResponseTriggerStateData >( );      break;
         case eOnOff::NotificationCurrentState:    ptr = std::make_shared< NotificationCurrentStateData >( );  break;
         default:                                                                                              break;
      }

      if( nullptr != ptr )
         ptr->from_stream( stream );

      return  ptr;
   }

   bool BaseData::serrialize( base::dsi::tByteStream& stream )
   {
      if( false == stream.push( id( ) ) )
         return false;

      return to_stream( stream );
   }



   const eOnOff RequestTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff RequestTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const size_t _delay )
      : BaseData( ), state( _state ), delay( _delay ) { }

   const eOnOff RequestTriggerStateData::ID = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::id( ) const { return ID; }

   bool RequestTriggerStateData::to_stream( base::dsi::tByteStream& stream ) { return stream.push( state, delay ); }
   bool RequestTriggerStateData::from_stream( base::dsi::tByteStream& stream ) { return stream.pop( state, delay ); }



   const eOnOff ResponseTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff ResponseTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff ResponseTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   ResponseTriggerStateData::ResponseTriggerStateData( const bool _result )
      : BaseData( ), result( _result ) { }

   const eOnOff ResponseTriggerStateData::ID = eOnOff::ResponseTriggerState;
   const eOnOff ResponseTriggerStateData::id( ) const { return ID; }

   bool ResponseTriggerStateData::to_stream( base::dsi::tByteStream& stream ) { return stream.push( result ); }
   bool ResponseTriggerStateData::from_stream( base::dsi::tByteStream& stream ) { return stream.pop( result ); }



   const eOnOff RequestStartData::REQUEST = eOnOff::RequestStart;
   const eOnOff RequestStartData::RESPONSE = eOnOff::Undefined;
   const eOnOff RequestStartData::BUSY = eOnOff::RequestStartBusy;

   const eOnOff RequestStartData::ID = eOnOff::RequestTriggerState;
   const eOnOff RequestStartData::id( ) const { return ID; }

   bool RequestStartData::to_stream( base::dsi::tByteStream& stream ) { return true; }
   bool RequestStartData::from_stream( base::dsi::tByteStream& stream ) { return true; }



   const eOnOff NotificationCurrentStateData::SUBSCRIBE = eOnOff::SubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::UNSUBSCRIBE = eOnOff::UnsubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::NOTIFICATION = eOnOff::NotificationCurrentState;

   NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
      : BaseData( ), state( _state ) { }

   const eOnOff NotificationCurrentStateData::ID = eOnOff::NotificationCurrentState;
   const eOnOff NotificationCurrentStateData::id( ) const { return ID; }

   bool NotificationCurrentStateData::to_stream( base::dsi::tByteStream& stream ) { return stream.push( state ); }
   bool NotificationCurrentStateData::from_stream( base::dsi::tByteStream& stream ) { return stream.pop( state ); }



   OnOffEventData::OnOffEventData( tBaseDataPtr _ptr ) : ptr( _ptr ) { }

   bool OnOffEventData::to_stream( base::dsi::tByteStream& stream ) const
   {
      if( nullptr == ptr ) return true;
      return ptr->serrialize( stream );
   }
   bool OnOffEventData::from_stream( base::dsi::tByteStream& stream )
   {
      ptr = BaseData::create( stream );
      return nullptr != ptr;
   }

} // namespace base::onoff::ipc



namespace base::onoff::no_ipc {

   const std::vector< base::service::RequestResponseIDs< BaseTypes::tEventID > >& BaseTypes::RR = s_rr;
   const std::vector< base::service::NotificationIDs< BaseTypes::tEventID > >& BaseTypes::N = s_n;



   const eOnOff RequestTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff RequestTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const size_t _delay  )
      : BaseData( ), state( _state ), delay( _delay ) { }



   const eOnOff ResponseTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff ResponseTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff ResponseTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   ResponseTriggerStateData::ResponseTriggerStateData( const bool _result )
      : BaseData( ), result( _result ) { }



   const eOnOff RequestStartData::REQUEST = eOnOff::RequestStart;
   const eOnOff RequestStartData::RESPONSE = eOnOff::Undefined;
   const eOnOff RequestStartData::BUSY = eOnOff::RequestStartBusy;



   const eOnOff NotificationCurrentStateData::SUBSCRIBE = eOnOff::SubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::UNSUBSCRIBE = eOnOff::UnsubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::NOTIFICATION = eOnOff::NotificationCurrentState;

   NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
      : BaseData( ), state( _state ) { }



   OnOffEventData::OnOffEventData( tBaseDataPtr _ptr ) : ptr( _ptr ) { }

} // namespace base::onoff::no_ipc
