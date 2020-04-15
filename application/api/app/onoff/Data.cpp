// Application
#include "Data.hpp"



namespace api::onoff {

   const std::string interface_name = "OnOff";

   const std::vector< base::RequestResponseIDs< eOnOff > > s_rr = {
      { eOnOff::RequestTriggerState, eOnOff::RequestTriggerStateBusy, eOnOff::ResponseTriggerState },
      { eOnOff::RequestStart, eOnOff::Undefined, eOnOff::Undefined }
   };

   const std::vector< base::NotificationIDs< eOnOff > > s_n = {
      { eOnOff::SubscribeCurrentState, eOnOff::UnsubscribeCurrentState, eOnOff::NotificationCurrentState }
   };

} // namespace api::onoff



namespace api::onoff::ipc {

   const base::eCommType BaseTypes::COMM_TYPE = base::eCommType::IPC;
   const std::vector< base::RequestResponseIDs< BaseTypes::tEventID > >& BaseTypes::RR = s_rr;
   const std::vector< base::NotificationIDs< BaseTypes::tEventID > >& BaseTypes::N = s_n;



   tBaseDataPtr BaseData::create( base::ByteBufferT& buffer )
   {
      eOnOff id = eOnOff::Undefined;
      if( false == buffer.pop( id ) )
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
         ptr->from_buffer( buffer );

      return  ptr;
   }

   bool BaseData::serrialize( base::ByteBufferT& buffer )
   {
      if( false == to_buffer( buffer ) )
         return false;

      return buffer.push( id( ) );
   }



   const eOnOff RequestTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff RequestTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   RequestTriggerStateData::RequestTriggerStateData( const std::string& _state, const size_t _delay )
      : BaseData( ), state( _state ), delay( _delay ) { }

   const eOnOff RequestTriggerStateData::ID = eOnOff::RequestTriggerState;
   const eOnOff RequestTriggerStateData::id( ) const { return ID; }

   bool RequestTriggerStateData::to_buffer( base::ByteBufferT& buffer ) { return buffer.push( state, delay ); }
   bool RequestTriggerStateData::from_buffer( base::ByteBufferT& buffer ) { return buffer.pop( delay, state ); }



   const eOnOff ResponseTriggerStateData::REQUEST = eOnOff::RequestTriggerState;
   const eOnOff ResponseTriggerStateData::RESPONSE = eOnOff::ResponseTriggerState;
   const eOnOff ResponseTriggerStateData::BUSY = eOnOff::RequestTriggerStateBusy;

   ResponseTriggerStateData::ResponseTriggerStateData( const bool _result )
      : BaseData( ), result( _result ) { }

   const eOnOff ResponseTriggerStateData::ID = eOnOff::ResponseTriggerState;
   const eOnOff ResponseTriggerStateData::id( ) const { return ID; }

   bool ResponseTriggerStateData::to_buffer( base::ByteBufferT& buffer ) { return buffer.push( result ); }
   bool ResponseTriggerStateData::from_buffer( base::ByteBufferT& buffer ) { return buffer.pop( result ); }



   const eOnOff RequestStartData::REQUEST = eOnOff::RequestStart;
   const eOnOff RequestStartData::RESPONSE = eOnOff::Undefined;
   const eOnOff RequestStartData::BUSY = eOnOff::Undefined;

   const eOnOff RequestStartData::ID = eOnOff::RequestTriggerState;
   const eOnOff RequestStartData::id( ) const { return ID; }

   bool RequestStartData::to_buffer( base::ByteBufferT& buffer ) { return true; }
   bool RequestStartData::from_buffer( base::ByteBufferT& buffer ) { return true; }



   const eOnOff NotificationCurrentStateData::SUBSCRIBE = eOnOff::SubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::UNSUBSCRIBE = eOnOff::UnsubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::NOTIFICATION = eOnOff::NotificationCurrentState;

   NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
      : BaseData( ), state( _state ) { }

   const eOnOff NotificationCurrentStateData::ID = eOnOff::NotificationCurrentState;
   const eOnOff NotificationCurrentStateData::id( ) const { return ID; }

   bool NotificationCurrentStateData::to_buffer( base::ByteBufferT& buffer ) { return buffer.push( state ); }
   bool NotificationCurrentStateData::from_buffer( base::ByteBufferT& buffer ) { return buffer.pop( state ); }



   OnOffEventData::OnOffEventData( tBaseDataPtr _ptr ) : ptr( _ptr ) { }

   bool OnOffEventData::to_buffer( base::ByteBufferT& buffer ) const
   {
      if( nullptr == ptr ) return true;
      return ptr->serrialize( buffer );
   }
   bool OnOffEventData::from_buffer( base::ByteBufferT& buffer )
   {
      ptr = BaseData::create( buffer );
      return nullptr != ptr;
   }

} // namespace api::onoff::ipc



namespace api::onoff::no_ipc {

   const base::eCommType BaseTypes::COMM_TYPE = base::eCommType::ITC;
   const std::vector< base::RequestResponseIDs< BaseTypes::tEventID > >& BaseTypes::RR = s_rr;
   const std::vector< base::NotificationIDs< BaseTypes::tEventID > >& BaseTypes::N = s_n;



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
   const eOnOff RequestStartData::BUSY = eOnOff::Undefined;



   const eOnOff NotificationCurrentStateData::SUBSCRIBE = eOnOff::SubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::UNSUBSCRIBE = eOnOff::UnsubscribeCurrentState;
   const eOnOff NotificationCurrentStateData::NOTIFICATION = eOnOff::NotificationCurrentState;

   NotificationCurrentStateData::NotificationCurrentStateData( const std::string& _state )
      : BaseData( ), state( _state ) { }



   OnOffEventData::OnOffEventData( tBaseDataPtr _ptr ) : ptr( _ptr ) { }

} // namespace api::onoff::no_ipc
