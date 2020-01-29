#pragma once

// Framework
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace api::onoff {

   extern const std::string interface_name;

   DEFINE_ENUM( eOnOff, size_t, RequestTriggerState, ResponseTriggerState, NotificationCurrentState, Undefined );

}

namespace api::onoff::ipc {

   class BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   class BaseData
   {
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
      bool to_buffer( base::ByteBufferT& ) override;
      bool from_buffer( base::ByteBufferT& ) override;

   public:
      std::string state = "";
   };



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

} // namespace api::onoff::ipc



namespace api::onoff::no_ipc {

   struct BaseData;
   using tBaseDataPtr = std::shared_ptr< BaseData >;



   struct BaseData
   {
   };



   struct RequestTriggerStateData : public BaseData
   {
      RequestTriggerStateData( const std::string&, const size_t );

      std::string state = "";
      size_t delay = 0;
   };



   struct ResponseTriggerStateData : public BaseData
   {
      ResponseTriggerStateData( const bool );

      bool result = false;
   };



   struct NotificationCurrentStateData : public BaseData
   {
      NotificationCurrentStateData( const std::string& );

      std::string state = "";
   };



   struct OnOffEventData
   {
      OnOffEventData( tBaseDataPtr );

      tBaseDataPtr ptr = nullptr;
   };



   DECLARE_EVENT_RR( OnOffEvent, OnOffEventData, api::onoff::eOnOff );

} // namespace api::onoff::no_ipc
