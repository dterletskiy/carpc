#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/interface/Types.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace api::onoff {

   DEFINE_ENUM( eOnOff, size_t
      , RequestTriggerState, RequestTriggerStateBusy, ResponseTriggerState
      , SubscribeCurrentState, UnsubscribeCurrentState, NotificationCurrentState
      , Undefined
   );

   extern const std::vector< base::RequestResponse< eOnOff > > s_rr;
   extern const std::vector< base::Notification< eOnOff > > s_n;

} // namespace api::onoff



namespace api::onoff::ipc {

   struct BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   struct OnOffEventData
   {
      OnOffEventData( ) = default;
      OnOffEventData( tBaseDataPtr );

      tBaseDataPtr ptr = nullptr;

   public:
      bool to_buffer( base::ByteBufferT& ) const;
      bool from_buffer( base::ByteBufferT& );
   };
   DECLARE_IPC_EVENT_RR( OnOffEvent, OnOffEventData, api::onoff::eOnOff );



   struct Types
   {
      using tEvent = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tEventData = OnOffEvent::Data;
      using tEventID = OnOffEvent::ID;

      static const base::eCommType COMM_TYPE;
      static const std::vector< base::RequestResponse< tEventID > >& RR;
      static const std::vector< base::Notification< tEventID > >& N;
   };



   struct BaseData : public Types
   {
      BaseData( ) = default;
      virtual ~BaseData( ) = default;

   public:
      static tBaseDataPtr create( base::ByteBufferT& );
      bool serrialize( base::ByteBufferT& );

   private:
      virtual bool to_buffer( base::ByteBufferT& ) = 0;
      virtual bool from_buffer( base::ByteBufferT& ) = 0;
      virtual const eOnOff id( ) const = 0;
   };



   struct RequestTriggerStateData : public BaseData
   {
      static const eOnOff REQUEST;
      static const eOnOff RESPONSE;
      static const eOnOff BUSY;

      RequestTriggerStateData( ) = default;
      RequestTriggerStateData( const std::string&, const size_t );
      ~RequestTriggerStateData( ) override = default;

      std::string state = "";
      size_t delay = 0;

   private:
      bool to_buffer( base::ByteBufferT& ) override;
      bool from_buffer( base::ByteBufferT& ) override;
      const eOnOff id( ) const override;
      static const eOnOff ID;
   };



   struct ResponseTriggerStateData : public BaseData
   {
      static const eOnOff REQUEST;
      static const eOnOff RESPONSE;
      static const eOnOff BUSY;

      ResponseTriggerStateData( ) = default;
      ResponseTriggerStateData( const bool );
      ~ResponseTriggerStateData( ) override = default;

      bool result = false;

   private:
      bool to_buffer( base::ByteBufferT& ) override;
      bool from_buffer( base::ByteBufferT& ) override;
      const eOnOff id( ) const override;
      static const eOnOff ID;
   };



   struct NotificationCurrentStateData : public BaseData
   {
      static const eOnOff SUBSCRIBE;
      static const eOnOff UNSUBSCRIBE;
      static const eOnOff NOTIFICATION;

      NotificationCurrentStateData( ) = default;
      NotificationCurrentStateData( const std::string& );
      ~NotificationCurrentStateData( ) override = default;

      std::string state = "";

   private:
      bool to_buffer( base::ByteBufferT& ) override;
      bool from_buffer( base::ByteBufferT& ) override;
      const eOnOff id( ) const override;
      static const eOnOff ID;
   };

} // namespace api::onoff::ipc



namespace api::onoff::no_ipc {

   struct BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   struct OnOffEventData
   {
      OnOffEventData( ) = default;
      OnOffEventData( tBaseDataPtr );

      tBaseDataPtr ptr = nullptr;
   };
   DECLARE_EVENT_RR( OnOffEvent, OnOffEventData, api::onoff::eOnOff );



   struct Types
   {
      using tEvent = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tEventData = OnOffEvent::Data;
      using tEventID = OnOffEvent::ID;

      static const base::eCommType COMM_TYPE;
      static const std::vector< base::RequestResponse< tEventID > >& RR;
      static const std::vector< base::Notification< tEventID > >& N;
   };



   struct BaseData : public Types
   {
      BaseData( ) = default;
      virtual ~BaseData( ) = default;
   };



   struct RequestTriggerStateData : public BaseData
   {
      static const eOnOff REQUEST;
      static const eOnOff RESPONSE;
      static const eOnOff BUSY;

      RequestTriggerStateData( ) = default;
      RequestTriggerStateData( const std::string&, const size_t );
      ~RequestTriggerStateData( ) override = default;

      std::string state = "";
      size_t delay = 0;
   };



   struct ResponseTriggerStateData : public BaseData
   {
      static const eOnOff REQUEST;
      static const eOnOff RESPONSE;
      static const eOnOff BUSY;

      ResponseTriggerStateData( ) = default;
      ResponseTriggerStateData( const bool );
      ~ResponseTriggerStateData( ) override = default;

      bool result = false;
   };



   struct NotificationCurrentStateData : public BaseData
   {
      static const eOnOff SUBSCRIBE;
      static const eOnOff UNSUBSCRIBE;
      static const eOnOff NOTIFICATION;

      NotificationCurrentStateData( ) = default;
      NotificationCurrentStateData( const std::string& );
      ~NotificationCurrentStateData( ) override = default;

      std::string state = "";
   };

} // namespace api::onoff::no_ipc



namespace api::onoff {

   namespace data = api::onoff::ipc;

   extern const std::string interface_name;

} // namespace api::onoff
