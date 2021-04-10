#pragma once

#define FAST 0
#define SECURE 1
#define SERVICE_TYPE FAST

#if SERVICE_TYPE == FAST
   #define SERVICE_NAMESPACE fast
#elif SERVICE_TYPE == SECURE
   #define SERVICE_NAMESPACE secure
#endif

#include "api/sys/comm/service/Types.hpp"
#include "api/sys/helpers/macros/enum.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace base::onoff {

   DEFINE_ENUM( eOnOff, std::size_t
      , RequestTriggerState, RequestTriggerStateBusy, ResponseTriggerState
      , RequestStart, RequestStartBusy
      , SubscribeCurrentState, UnsubscribeCurrentState, NotificationCurrentState
      , Undefined // Must present always for all interfaces
   );

} // namespace base::onoff



namespace base::onoff::ipc {

   class BaseData
   {
      public:
         using tSptr = std::shared_ptr< BaseData >;

         BaseData( ) = default;
         virtual ~BaseData( ) = default;

      public:
         static tSptr create( base::ipc::tStream& );
         bool serrialize( base::ipc::tStream& );

      private:
         virtual bool to_stream( base::ipc::tStream& ) = 0;
         virtual bool from_stream( base::ipc::tStream& ) = 0;
         virtual const eOnOff id( ) const = 0;
   };



   class RequestTriggerStateData : public BaseData
   {
      public:
         static const eOnOff REQUEST;
         static const eOnOff RESPONSE;
         static const eOnOff BUSY;

      public:
         RequestTriggerStateData( ) = default;
         RequestTriggerStateData( const std::string&, const std::size_t );
         ~RequestTriggerStateData( ) override = default;

         std::string state = "";
         std::size_t delay = 0;

      private:
         bool to_stream( base::ipc::tStream& ) override;
         bool from_stream( base::ipc::tStream& ) override;
         const eOnOff id( ) const override;
         static const eOnOff ID;
   };



   class ResponseTriggerStateData : public BaseData
   {
      public:
         static const eOnOff REQUEST;
         static const eOnOff RESPONSE;
         static const eOnOff BUSY;

      public:
         ResponseTriggerStateData( ) = default;
         ResponseTriggerStateData( const bool );
         ~ResponseTriggerStateData( ) override = default;

         bool result = false;

      private:
         bool to_stream( base::ipc::tStream& ) override;
         bool from_stream( base::ipc::tStream& ) override;
         const eOnOff id( ) const override;
         static const eOnOff ID;
   };



   class RequestStartData : public BaseData
   {
      public:
         static const eOnOff REQUEST;
         static const eOnOff RESPONSE;
         static const eOnOff BUSY;

      public:
         RequestStartData( ) = default;
         ~RequestStartData( ) override = default;

      private:
         bool to_stream( base::ipc::tStream& ) override;
         bool from_stream( base::ipc::tStream& ) override;
         const eOnOff id( ) const override;
         static const eOnOff ID;
   };



   class NotificationCurrentStateData : public BaseData
   {
      public:
         static const eOnOff SUBSCRIBE;
         static const eOnOff UNSUBSCRIBE;
         static const eOnOff NOTIFICATION;

      public:
         NotificationCurrentStateData( ) = default;
         NotificationCurrentStateData( const std::string& );
         ~NotificationCurrentStateData( ) override = default;

         std::string state = "";

      private:
         bool to_stream( base::ipc::tStream& ) override;
         bool from_stream( base::ipc::tStream& ) override;
         const eOnOff id( ) const override;
         static const eOnOff ID;
   };



   struct Types
   {
      using tIPC           = base::service::IPC;
      using tID            = eOnOff;
      using tBaseData      = BaseData;

      static const base::service::RequestResponseIDs< tID >::tVector& RR;
      static const base::service::NotificationIDs< tID >::tVector& N;
   };

} // namespace base::onoff::ipc



namespace base::onoff::no_ipc {

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
         static const eOnOff REQUEST;
         static const eOnOff RESPONSE;
         static const eOnOff BUSY;

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
         static const eOnOff REQUEST;
         static const eOnOff RESPONSE;
         static const eOnOff BUSY;

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
         static const eOnOff REQUEST;
         static const eOnOff RESPONSE;
         static const eOnOff BUSY;

      public:
         RequestStartData( ) = default;
         ~RequestStartData( ) override = default;
   };



   class NotificationCurrentStateData : public BaseData
   {
      public:
         static const eOnOff SUBSCRIBE;
         static const eOnOff UNSUBSCRIBE;
         static const eOnOff NOTIFICATION;

      public:
         NotificationCurrentStateData( ) = default;
         NotificationCurrentStateData( const std::string& );
         ~NotificationCurrentStateData( ) override = default;

      public:
         std::string state = "";
   };



   struct Types
   {
      using tIPC           = base::service::NO_IPC;
      using tID            = eOnOff;
      using tBaseData      = BaseData;

      static const base::service::RequestResponseIDs< tID >::tVector& RR;
      static const base::service::NotificationIDs< tID >::tVector& N;
   };

} // namespace base::onoff::no_ipc



namespace base::onoff {

   namespace data = base::onoff::ipc;

} // namespace base::onoff
