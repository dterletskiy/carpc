#include "Data.hpp"



namespace service::onoff::V1_0_0 {

   const carpc::service::RequestResponseIDs< eOnOff >::tVector s_rr = {
      { eOnOff::RequestTriggerState, eOnOff::RequestTriggerStateBusy, eOnOff::ResponseTriggerState },
      { eOnOff::RequestStart, eOnOff::RequestStartBusy, eOnOff::Undefined }
   };

   const carpc::service::NotificationIDs< eOnOff >::tVector s_n = {
      { eOnOff::SubscribeCurrentState, eOnOff::UnsubscribeCurrentState, eOnOff::NotificationCurrentState }
   };

} // namespace service::onoff::V1_0_0



namespace service::onoff::V1_0_0::ipc {

   const carpc::service::RequestResponseIDs< Types::tID >::tVector& Types::RR = s_rr;
   const carpc::service::NotificationIDs< Types::tID >::tVector& Types::N = s_n;



   BaseData::tSptr BaseData::create( carpc::ipc::tStream& stream )
   {
      eOnOff id = eOnOff::Undefined;
      if( false == carpc::ipc::deserialize( stream, id ) )
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

   bool BaseData::serrialize( carpc::ipc::tStream& stream )
   {
      if( false == carpc::ipc::serialize( stream, id( ) ) )
         return false;

      return to_stream( stream );
   }



   RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const std::size_t _delay )
      : BaseData( ), state( _state ), delay( _delay ) { }

   bool RequestTriggerStateData::to_stream( carpc::ipc::tStream& stream ) { return carpc::ipc::serialize( stream, state, delay ); }
   bool RequestTriggerStateData::from_stream( carpc::ipc::tStream& stream ) { return carpc::ipc::deserialize( stream, state, delay ); }



   ResponseTriggerStateData::ResponseTriggerStateData( const bool _result )
      : BaseData( ), result( _result ) { }

   bool ResponseTriggerStateData::to_stream( carpc::ipc::tStream& stream ) { return carpc::ipc::serialize( stream, result ); }
   bool ResponseTriggerStateData::from_stream( carpc::ipc::tStream& stream ) { return carpc::ipc::deserialize( stream, result ); }



   bool RequestStartData::to_stream( carpc::ipc::tStream& stream ) { return true; }
   bool RequestStartData::from_stream( carpc::ipc::tStream& stream ) { return true; }



   NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
      : BaseData( ), state( _state ) { }

   bool NotificationCurrentStateData::to_stream( carpc::ipc::tStream& stream ) { return carpc::ipc::serialize( stream, state ); }
   bool NotificationCurrentStateData::from_stream( carpc::ipc::tStream& stream ) { return carpc::ipc::deserialize( stream, state ); }

} // namespace service::onoff::V1_0_0::ipc



namespace service::onoff::V1_0_0::no_ipc {

   const carpc::service::RequestResponseIDs< Types::tID >::tVector& Types::RR = s_rr;
   const carpc::service::NotificationIDs< Types::tID >::tVector& Types::N = s_n;



   RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const std::size_t _delay  )
      : BaseData( ), state( _state ), delay( _delay ) { }



   ResponseTriggerStateData::ResponseTriggerStateData( const bool _result )
      : BaseData( ), result( _result ) { }



   NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
      : BaseData( ), state( _state ) { }

} // namespace service::onoff::V1_0_0::no_ipc
