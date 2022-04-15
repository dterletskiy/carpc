#pragma once

#define FAST 0
#define SECURE 1
#define SERVICE_TYPE FAST

#if SERVICE_TYPE == FAST
   #define SERVICE_NAMESPACE fast
#elif SERVICE_TYPE == SECURE
   #define SERVICE_NAMESPACE secure
#endif

#include <memory>

#include "api/sys/helpers/macros/enum.hpp"
#include "api/sys/helpers/macros/strings.hpp"
#include "api/service/onoff/1.0.0/Version.hpp"



namespace service::onoff::V1_0_0 {

   DEFINE_ENUMERATION( eOnOff, std::size_t
      , RequestTriggerState, RequestTriggerStateBusy, ResponseTriggerState
      , RequestStart, RequestStartBusy
      , SubscribeCurrentState, UnsubscribeCurrentState, NotificationCurrentState
      , Undefined // Must present always for all interfaces
   );

}



namespace service::onoff::V1_0_0::ipc {

   class BaseData
   {
      public:
         using tSptr = std::shared_ptr< BaseData >;

         BaseData( ) = default;
         virtual ~BaseData( ) = default;

      public:
         static tSptr create( carpc::ipc::tStream& );
         bool serrialize( carpc::ipc::tStream& );

      private:
         virtual bool to_stream( carpc::ipc::tStream& ) = 0;
         virtual bool from_stream( carpc::ipc::tStream& ) = 0;
         virtual const eOnOff id( ) const = 0;
   };



   class RequestTriggerStateData : public BaseData
   {
      public:
         static constexpr eOnOff REQUEST = eOnOff::RequestTriggerState;
         static constexpr eOnOff RESPONSE = eOnOff::ResponseTriggerState;
         static constexpr eOnOff BUSY = eOnOff::RequestTriggerStateBusy;

      public:
         RequestTriggerStateData( ) = default;
         RequestTriggerStateData( const std::string&, const std::size_t );
         ~RequestTriggerStateData( ) override = default;

         std::string state = "";
         std::size_t delay = 0;

      private:
         bool to_stream( carpc::ipc::tStream& ) override;
         bool from_stream( carpc::ipc::tStream& ) override;
         const eOnOff id( ) const override { return ID; }
         static constexpr eOnOff ID = eOnOff::RequestTriggerState;
   };



   class ResponseTriggerStateData : public BaseData
   {
      public:
         static constexpr eOnOff REQUEST = eOnOff::RequestTriggerState;
         static constexpr eOnOff RESPONSE = eOnOff::ResponseTriggerState;
         static constexpr eOnOff BUSY = eOnOff::RequestTriggerStateBusy;

      public:
         ResponseTriggerStateData( ) = default;
         ResponseTriggerStateData( const bool );
         ~ResponseTriggerStateData( ) override = default;

         bool result = false;

      private:
         bool to_stream( carpc::ipc::tStream& ) override;
         bool from_stream( carpc::ipc::tStream& ) override;
         const eOnOff id( ) const override { return ID; }
         static constexpr eOnOff ID = eOnOff::ResponseTriggerState;
   };



   class RequestStartData : public BaseData
   {
      public:
         static constexpr eOnOff REQUEST = eOnOff::RequestStart;
         static constexpr eOnOff RESPONSE = eOnOff::Undefined;
         static constexpr eOnOff BUSY = eOnOff::RequestStartBusy;

      public:
         RequestStartData( ) = default;
         ~RequestStartData( ) override = default;

      private:
         bool to_stream( carpc::ipc::tStream& ) override;
         bool from_stream( carpc::ipc::tStream& ) override;
         const eOnOff id( ) const override { return ID; }
         static constexpr eOnOff ID = eOnOff::RequestTriggerState;
   };



   class NotificationCurrentStateData : public BaseData
   {
      public:
         static constexpr eOnOff SUBSCRIBE = eOnOff::SubscribeCurrentState;
         static constexpr eOnOff UNSUBSCRIBE = eOnOff::UnsubscribeCurrentState;
         static constexpr eOnOff NOTIFICATION = eOnOff::NotificationCurrentState;

      public:
         NotificationCurrentStateData( ) = default;
         NotificationCurrentStateData( const std::string& );
         ~NotificationCurrentStateData( ) override = default;

         std::string state = "";

      private:
         bool to_stream( carpc::ipc::tStream& ) override;
         bool from_stream( carpc::ipc::tStream& ) override;
         const eOnOff id( ) const override { return ID; }
         static constexpr eOnOff ID = eOnOff::NotificationCurrentState;
   };



   struct Types
   {
      using tIPC           = carpc::service::IPC;
      using tID            = eOnOff;
      using tBaseData      = BaseData;

      static const carpc::service::RequestResponseIDs< tID >::tVector& RR;
      static const carpc::service::NotificationIDs< tID >::tVector& N;
   };

} // namespace service::onoff::V1_0_0::ipc



namespace service::onoff::V1_0_0::no_ipc {

   class BaseData
   {
      public:
         using tSptr = std::shared_ptr< BaseData >;

         BaseData( ) = default;
         virtual ~BaseData( ) = default;
   };



   class RequestTriggerStateData : public BaseData
   {
      public:
         static constexpr eOnOff REQUEST = eOnOff::RequestTriggerState;
         static constexpr eOnOff RESPONSE = eOnOff::ResponseTriggerState;
         static constexpr eOnOff BUSY = eOnOff::RequestTriggerStateBusy;

      public:
         RequestTriggerStateData( ) = default;
         RequestTriggerStateData( const std::string&, const std::size_t );
         ~RequestTriggerStateData( ) override = default;

      public:
         std::string state = "";
         std::size_t delay = 0;
   };



   class ResponseTriggerStateData : public BaseData
   {
      public:
         static constexpr eOnOff REQUEST = eOnOff::RequestTriggerState;
         static constexpr eOnOff RESPONSE = eOnOff::ResponseTriggerState;
         static constexpr eOnOff BUSY = eOnOff::RequestTriggerStateBusy;

      public:
         ResponseTriggerStateData( ) = default;
         ResponseTriggerStateData( const bool );
         ~ResponseTriggerStateData( ) override = default;

      public:
         bool result = false;
   };



   class RequestStartData : public BaseData
   {
      public:
         static constexpr eOnOff REQUEST = eOnOff::RequestStart;
         static constexpr eOnOff RESPONSE = eOnOff::Undefined;
         static constexpr eOnOff BUSY = eOnOff::RequestStartBusy;

      public:
         RequestStartData( ) = default;
         ~RequestStartData( ) override = default;
   };



   class NotificationCurrentStateData : public BaseData
   {
      public:
         static constexpr eOnOff SUBSCRIBE = eOnOff::SubscribeCurrentState;
         static constexpr eOnOff UNSUBSCRIBE = eOnOff::UnsubscribeCurrentState;
         static constexpr eOnOff NOTIFICATION = eOnOff::NotificationCurrentState;

      public:
         NotificationCurrentStateData( ) = default;
         NotificationCurrentStateData( const std::string& );
         ~NotificationCurrentStateData( ) override = default;

      public:
         std::string state = "";
   };



   struct Types
   {
      using tIPC           = carpc::service::NO_IPC;
      using tID            = eOnOff;
      using tBaseData      = BaseData;

      static const carpc::service::RequestResponseIDs< tID >::tVector& RR;
      static const carpc::service::NotificationIDs< tID >::tVector& N;
   };

} // namespace service::onoff::V1_0_0::no_ipc



namespace service::onoff::V1_0_0 {

   namespace data = service::onoff::V1_0_0::ipc;

} // namespace service::onoff::V1_0_0
