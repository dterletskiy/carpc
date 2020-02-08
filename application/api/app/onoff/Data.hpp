#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace api::onoff {
   DEFINE_ENUM( eOnOff, size_t, RequestTriggerState, ResponseTriggerState, NotificationCurrentState, Undefined );
} // namespace api::onoff



namespace api::onoff::ipc {

   extern const bool is_ipc;

   class BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   class OnOffEventData
   {
   public:
      OnOffEventData( );
      OnOffEventData( tBaseDataPtr );

   public:
      bool to_buffer( base::ByteBufferT& ) const;
      bool from_buffer( base::ByteBufferT& );

   public:
      tBaseDataPtr ptr = nullptr;
   };
   DECLARE_IPC_EVENT_RR( OnOffEvent, OnOffEventData, api::onoff::eOnOff );



   class BaseData
   {
   public:
      using tEvent = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tEventData = OnOffEvent::Data;
      using tEventID = OnOffEvent::ID;

   public:
      BaseData( const eOnOff );
      static tBaseDataPtr create( base::ByteBufferT& );

   public:
      virtual bool to_buffer( base::ByteBufferT& ) = 0;
      virtual bool from_buffer( base::ByteBufferT& ) = 0;

   public:
      virtual const eOnOff id( ) const final;
   private:
      eOnOff m_id = eOnOff::Undefined;
   };



   class RequestTriggerStateData : public BaseData
   {
   public:
      RequestTriggerStateData( );
      RequestTriggerStateData( const std::string&, const size_t );
   public:
      static const eOnOff id;

   public:
      bool to_buffer( base::ByteBufferT& ) override;
      bool from_buffer( base::ByteBufferT& ) override;

   public:
      std::string state = "";
      size_t delay = 0;
   };



   class ResponseTriggerStateData : public BaseData
   {
   public:
      ResponseTriggerStateData( );
      ResponseTriggerStateData( const bool );
   public:
      static const eOnOff id;

   public:
      bool to_buffer( base::ByteBufferT& ) override;
      bool from_buffer( base::ByteBufferT& ) override;

   public:
      bool result = false;
   };



   class NotificationCurrentStateData : public BaseData
   {
   public:
      NotificationCurrentStateData( );
      NotificationCurrentStateData( const std::string& );
   public:
      static const eOnOff id;

   public:
      bool to_buffer( base::ByteBufferT& ) override;
      bool from_buffer( base::ByteBufferT& ) override;

   public:
      std::string state = "";
   };

} // namespace api::onoff::ipc



namespace api::onoff::no_ipc {

   extern const bool is_ipc;

   struct BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   struct OnOffEventData
   {
      OnOffEventData( tBaseDataPtr );

      tBaseDataPtr ptr = nullptr;
   };
   DECLARE_EVENT_RR( OnOffEvent, OnOffEventData, api::onoff::eOnOff );



   struct BaseData
   {
      using tEvent = OnOffEvent::Event;
      using tEventConsumer = OnOffEvent::Consumer;
      using tEventData = OnOffEvent::Data;
      using tEventID = OnOffEvent::ID;
   };



   struct RequestTriggerStateData : public BaseData
   {
      RequestTriggerStateData( const std::string&, const size_t );
      static const eOnOff id;

      std::string state = "";
      size_t delay = 0;
   };



   struct ResponseTriggerStateData : public BaseData
   {
      ResponseTriggerStateData( const bool );
      static const eOnOff id;

      bool result = false;
   };



   struct NotificationCurrentStateData : public BaseData
   {
      NotificationCurrentStateData( const std::string& );
      static const eOnOff id;

      std::string state = "";
   };

} // namespace api::onoff::no_ipc



namespace api::onoff {

   namespace data = api::onoff::ipc;

   extern const std::string interface_name;
   extern const bool is_ipc;

} // namespace api::onoff
