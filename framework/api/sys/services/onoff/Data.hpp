#pragma once

#define FAST 0
#define SECURE 1
#define SERVICE_TYPE FAST

#if SERVICE_TYPE == FAST
   #define SERVICE_NAMESPACE fast
#elif SERVICE_TYPE == SECURE
   #define SERVICE_NAMESPACE secure
#endif

#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/comm/service/Types.hpp"
#include "api/sys/comm/service/fast/TSignature.hpp"
#include "api/sys/comm/service/secure/TSignature.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace base::onoff {

   DEFINE_ENUM( eOnOff, size_t
      , RequestTriggerState, RequestTriggerStateBusy, ResponseTriggerState
      , RequestStart, RequestStartBusy
      , SubscribeCurrentState, UnsubscribeCurrentState, NotificationCurrentState
      , Undefined // Must present always for all interfaces
   );
   using tSignature = base::service::SERVICE_NAMESPACE::TSignature< eOnOff >;

} // namespace base::onoff



namespace base::onoff::ipc {

   struct BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   struct OnOffEventData
   {
      OnOffEventData( ) = default;
      OnOffEventData( tBaseDataPtr );

      tBaseDataPtr ptr = nullptr;

   public:
      bool to_stream( base::dsi::tByteStream& ) const;
      bool from_stream( base::dsi::tByteStream& );
   };
   DEFINE_IPC_EVENT( OnOffEvent, OnOffEventData, base::onoff::tSignature );



   struct BaseTypes
   {
      using tEvent         = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tSignature     = OnOffEvent::UserSignature;
      using tEventID       = OnOffEvent::UserSignature::tID;
      using tEventData     = OnOffEvent::Data;
      using tBaseData      = BaseData;

      static const base::service::RequestResponseIDs< tEventID >::tVector& RR;
      static const base::service::NotificationIDs< tEventID >::tVector& N;
   };

   struct Types : public BaseTypes
   {
   };



   struct BaseData : public BaseTypes
   {
      BaseData( ) = default;
      virtual ~BaseData( ) = default;

   public:
      static tBaseDataPtr create( base::dsi::tByteStream& );
      bool serrialize( base::dsi::tByteStream& );

   private:
      virtual bool to_stream( base::dsi::tByteStream& ) = 0;
      virtual bool from_stream( base::dsi::tByteStream& ) = 0;
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
      bool to_stream( base::dsi::tByteStream& ) override;
      bool from_stream( base::dsi::tByteStream& ) override;
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
      bool to_stream( base::dsi::tByteStream& ) override;
      bool from_stream( base::dsi::tByteStream& ) override;
      const eOnOff id( ) const override;
      static const eOnOff ID;
   };



   struct RequestStartData : public BaseData
   {
      static const eOnOff REQUEST;
      static const eOnOff RESPONSE;
      static const eOnOff BUSY;

      RequestStartData( ) = default;
      ~RequestStartData( ) override = default;

   private:
      bool to_stream( base::dsi::tByteStream& ) override;
      bool from_stream( base::dsi::tByteStream& ) override;
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
      bool to_stream( base::dsi::tByteStream& ) override;
      bool from_stream( base::dsi::tByteStream& ) override;
      const eOnOff id( ) const override;
      static const eOnOff ID;
   };

} // namespace base::onoff::ipc



namespace base::onoff::no_ipc {

   struct BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   struct OnOffEventData
   {
      OnOffEventData( ) = default;
      OnOffEventData( tBaseDataPtr );

      tBaseDataPtr ptr = nullptr;
   };
   DEFINE_EVENT( OnOffEvent, OnOffEventData, base::onoff::tSignature );



   struct BaseTypes
   {
      using tEvent         = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tSignature     = OnOffEvent::UserSignature;
      using tEventID       = OnOffEvent::UserSignature::tID;
      using tEventData     = OnOffEvent::Data;
      using tBaseData      = BaseData;

      static const base::service::RequestResponseIDs< tEventID >::tVector& RR;
      static const base::service::NotificationIDs< tEventID >::tVector& N;
   };

   struct Types : public BaseTypes
   {
   };



   struct BaseData : public BaseTypes
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



   struct RequestStartData : public BaseData
   {
      static const eOnOff REQUEST;
      static const eOnOff RESPONSE;
      static const eOnOff BUSY;

      RequestStartData( ) = default;
      ~RequestStartData( ) override = default;
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

} // namespace base::onoff::no_ipc



namespace base::onoff {

   namespace data = base::onoff::ipc;

   extern const base::async::tAsyncTypeID interface_type_id;

} // namespace base::onoff
