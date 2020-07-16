#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/interface/Types.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace api::onoff {

   DEFINE_ENUM( eOnOff, size_t
      , RequestTriggerState, RequestTriggerStateBusy, ResponseTriggerState
      , RequestStart
      , SubscribeCurrentState, UnsubscribeCurrentState, NotificationCurrentState
      , Undefined // Must present always for all interfaces
   );
   using tSignature = base::TSignatureRR< eOnOff >;

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
      bool to_stream( base::dsi::tByteStream& ) const;
      bool from_stream( base::dsi::tByteStream& );
   };
   DEFINE_IPC_EVENT( OnOffEvent, OnOffEventData, api::onoff::tSignature );



   struct BaseTypes
   {
      using tEvent = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tSignature = api::onoff::tSignature;
      using tEventID = api::onoff::eOnOff;
      using tBaseData = BaseData;
      using tEventData = OnOffEventData;

      static const base::eCommType COMM_TYPE;
      static const std::vector< base::RequestResponseIDs< tEventID > >& RR;
      static const std::vector< base::NotificationIDs< tEventID > >& N;
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

   struct Types : public BaseTypes
   {
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
   DEFINE_EVENT( OnOffEvent, OnOffEventData, api::onoff::tSignature );



   struct BaseTypes
   {
      using tEvent = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tSignature = api::onoff::tSignature;
      using tEventID = api::onoff::eOnOff;
      using tBaseData = BaseData;
      using tEventData = OnOffEventData;

      static const base::eCommType COMM_TYPE;
      static const std::vector< base::RequestResponseIDs< tEventID > >& RR;
      static const std::vector< base::NotificationIDs< tEventID > >& N;
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

   struct Types : public BaseTypes
   {
   };

} // namespace api::onoff::no_ipc



namespace api::onoff {

   namespace data = api::onoff::ipc;

   extern const std::string interface_name;

} // namespace api::onoff