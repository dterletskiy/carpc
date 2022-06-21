#pragma once

#include "carpc/comm/async/event/TGenerator.hpp"
#include "carpc/helpers/macros/enum.hpp"




#define DEFINE_EVENT_BASE( scopeType, serviceType, eventType, dataType, signatureType ) \
   scopeType eventType { \
      class eventType##_TYPE; \
      using Generator      = carpc::async::TGenerator< serviceType, eventType##_TYPE, dataType, signatureType >; \
      using Event          = typename Generator::Config::tEvent; \
      using Signature      = typename Generator::Config::tSignature; \
      using UserSignature  = typename Generator::Config::tUserSignature; \
      using Data           = typename Generator::Config::tData; \
      using Consumer       = typename Generator::Config::tConsumer; \
   }



#define DEFINE_EVENT_N( eventType, dataType, signatureType ) \
   DEFINE_EVENT_BASE( namespace, carpc::NO_IPC, eventType, dataType, signatureType )

#define DEFINE_EVENT_SIMPLE_N( eventType, dataType ) \
   DEFINE_EVENT_N( eventType, dataType, carpc::async::simple::Signature )

#define DEFINE_EVENT_ID_N( eventType, enumType ) \
   DEFINE_EVENT_N( eventType, dataType, carpc::async::id::TSignature< enumType > )

#define DEFINE_IPC_EVENT_N( eventType, dataType, signatureType ) \
   DEFINE_EVENT_BASE( namespace, carpc::IPC, eventType, dataType, signatureType )

#define DEFINE_IPC_EVENT_SIMPLE_N( eventType, dataType ) \
   DEFINE_IPC_EVENT_N( eventType, dataType, carpc::async::simple::Signature )

#define DEFINE_IPC_EVENT_ID_N( eventType, enumType ) \
   DEFINE_IPC_EVENT_N( eventType, dataType, carpc::async::id::TSignature< enumType > )



#define DEFINE_EVENT_S( eventType, dataType, signatureType ) \
   DEFINE_EVENT_BASE( struct, carpc::NO_IPC, eventType, dataType, signatureType )

#define DEFINE_EVENT_SIMPLE_S( eventType, dataType ) \
   DEFINE_EVENT_S( eventType, dataType, carpc::async::simple::Signature )

#define DEFINE_EVENT_ID_S( eventType, dataType, enumType ) \
   DEFINE_EVENT_S( eventType, dataType, carpc::async::id::TSignature< enumType > )

#define DEFINE_IPC_EVENT_S( eventType, dataType, signatureType ) \
   DEFINE_EVENT_BASE( struct, carpc::IPC, eventType, dataType, signatureType )

#define DEFINE_IPC_EVENT_SIMPLE_S( eventType, dataType ) \
   DEFINE_IPC_EVENT_S( eventType, dataType, carpc::async::simple::Signature )

#define DEFINE_IPC_EVENT_ID_S( eventType, dataType, enumType ) \
   DEFINE_IPC_EVENT_S( eventType, dataType, carpc::async::id::TSignature< enumType > )



#define DEFINE_EVENT             DEFINE_EVENT_S
#define DEFINE_EVENT_SIMPLE      DEFINE_EVENT_SIMPLE_S
#define DEFINE_EVENT_ID          DEFINE_EVENT_ID_S

#define DEFINE_IPC_EVENT         DEFINE_IPC_EVENT_S
#define DEFINE_IPC_EVENT_SIMPLE  DEFINE_IPC_EVENT_SIMPLE_S
#define DEFINE_IPC_EVENT_ID      DEFINE_IPC_EVENT_ID_S



// There is alternative method to set event type id manually.
// In this case static const s_type_id field should be introduced to Signature.
// Additionally this macro must be called outside of any manespace and eventType parameter must contain
// namespace where corresponding event is defined. This is because of s_type_id must containe unique value.
// REGISTER_EVENT macro must use { #eventType } instead of build_type_id( ).
// #define INIT_EVENT( eventType ) \
//    template< > const carpc::async::tAsyncTypeID eventType::Signature::s_type_id = { #eventType };



#define REGISTER_EVENT( eventType ) \
   carpc::async::IEvent::check_in( eventType::Signature::build_type_id( ), eventType::Event::create );

#define DUMP_IPC_EVENTS \
   carpc::async::IEvent::dump( );
