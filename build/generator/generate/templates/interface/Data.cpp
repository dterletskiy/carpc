#include "api/sys/services/onoff/Data.hpp"



namespace base::onoff {

   const base::service::RequestResponseIDs< eOnOff >::tVector s_rr = {
      { eOnOff::RequestTriggerState, eOnOff::RequestTriggerStateBusy, eOnOff::ResponseTriggerState },
      { eOnOff::RequestStart, eOnOff::RequestStartBusy, eOnOff::Undefined }
   };

   const base::service::NotificationIDs< eOnOff >::tVector s_n = {
      { eOnOff::SubscribeCurrentState, eOnOff::UnsubscribeCurrentState, eOnOff::NotificationCurrentState }
   };

} // namespace base::onoff



namespace base::onoff::ipc {

   const base::service::RequestResponseIDs< Types::tID >::tVector& Types::RR = s_rr;
   const base::service::NotificationIDs< Types::tID >::tVector& Types::N = s_n;



   BaseData::tSptr BaseData::create( base::ipc::tStream& stream )
   {
      eOnOff id = eOnOff::Undefined;
      if( false == base::ipc::deserialize( stream, id ) )
         return nullptr;

      tSptr ptr = nullptr;
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

   bool BaseData::serrialize( base::ipc::tStream& stream )
   {
      if( false == base::ipc::serialize( stream, id( ) ) )
         return false;

      return to_stream( stream );
   }



   const eOnOff RequestTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff RequestTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const std::size_t _delay )
      : BaseData( ), state( _state ), delay( _delay ) { }

   const eOnOff RequestTriggerStateData::ID = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::id( ) const { return ID; }

   bool RequestTriggerStateData::to_stream( base::ipc::tStream& stream ) { return base::ipc::serialize( stream, state, delay ); }
   bool RequestTriggerStateData::from_stream( base::ipc::tStream& stream ) { return base::ipc::deserialize( stream, state, delay ); }



   const eOnOff ResponseTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff ResponseTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff ResponseTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   ResponseTriggerStateData::ResponseTriggerStateData( const bool _result )
      : BaseData( ), result( _result ) { }

   const eOnOff ResponseTriggerStateData::ID = eOnOff::ResponseTriggerState;
   const eOnOff ResponseTriggerStateData::id( ) const { return ID; }

   bool ResponseTriggerStateData::to_stream( base::ipc::tStream& stream ) { return base::ipc::serialize( stream, result ); }
   bool ResponseTriggerStateData::from_stream( base::ipc::tStream& stream ) { return base::ipc::deserialize( stream, result ); }



   const eOnOff RequestStartData::REQUEST = eOnOff::RequestStart;
   const eOnOff RequestStartData::RESPONSE = eOnOff::Undefined;
   const eOnOff RequestStartData::BUSY = eOnOff::RequestStartBusy;

   const eOnOff RequestStartData::ID = eOnOff::RequestTriggerState;
   const eOnOff RequestStartData::id( ) const { return ID; }

   bool RequestStartData::to_stream( base::ipc::tStream& stream ) { return true; }
   bool RequestStartData::from_stream( base::ipc::tStream& stream ) { return true; }



   const eOnOff NotificationCurrentStateData::SUBSCRIBE = eOnOff::SubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::UNSUBSCRIBE = eOnOff::UnsubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::NOTIFICATION = eOnOff::NotificationCurrentState;

   NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
      : BaseData( ), state( _state ) { }

   const eOnOff NotificationCurrentStateData::ID = eOnOff::NotificationCurrentState;
   const eOnOff NotificationCurrentStateData::id( ) const { return ID; }

   bool NotificationCurrentStateData::to_stream( base::ipc::tStream& stream ) { return base::ipc::serialize( stream, state ); }
   bool NotificationCurrentStateData::from_stream( base::ipc::tStream& stream ) { return base::ipc::deserialize( stream, state ); }

} // namespace base::onoff::ipc



namespace base::onoff::no_ipc {

   const base::service::RequestResponseIDs< Types::tID >::tVector& Types::RR = s_rr;
   const base::service::NotificationIDs< Types::tID >::tVector& Types::N = s_n;



   const eOnOff RequestTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff RequestTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const std::size_t _delay  )
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

} // namespace base::onoff::no_ipc
