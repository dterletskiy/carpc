#pragma once

#include "api/sys/comm/async/event/TGenerator.hpp"
#include "api/sys/helpers/macros/enum.hpp"




#if 0

   #define DEFINE_EVENT_N( eventType, dataType, signatureType ) \
      namespace eventType { \
         class eventType; \
         using Generator      = carpc::async::TGenerator< carpc::async::NoServiceType, eventType, dataType, signatureType >; \
         using Event          = Generator::Config::tEvent; \
         using Signature      = Generator::Config::tSignature; \
         using UserSignature  = Generator::Config::tUserSignature; \
         using Data           = Generator::Config::tData; \
         using Consumer       = Generator::Config::tConsumer; \
      }

   #define DEFINE_IPC_EVENT_N( eventType, dataType, signatureType ) \
      namespace eventType { \
         class ServiceType; \
         class eventType; \
         using Generator      = carpc::async::TGenerator< carpc::async::ServiceType, eventType, dataType, signatureType >; \
         using Event          = Generator::Config::tEvent; \
         using Signature      = Generator::Config::tSignature; \
         using UserSignature  = Generator::Config::tUserSignature; \
         using Data           = Generator::Config::tData; \
         using Consumer       = Generator::Config::tConsumer; \
      }



   #define DEFINE_EVENT_S( eventType, dataType, signatureType ) \
      struct eventType { \
         class eventType##_TYPE; \
         using Generator      = carpc::async::TGenerator< carpc::async::NoServiceType, eventType##_TYPE, dataType, signatureType >; \
         using Event          = typename Generator::Config::tEvent; \
         using Signature      = typename Generator::Config::tSignature; \
         using UserSignature  = typename Generator::Config::tUserSignature; \
         using Data           = typename Generator::Config::tData; \
         using Consumer       = typename Generator::Config::tConsumer; \
      };

   #define DEFINE_IPC_EVENT_S( eventType, dataType, signatureType ) \
      struct eventType { \
         class ServiceType; \
         class eventType##_TYPE; \
         using Generator      = carpc::async::TGenerator< carpc::async::ServiceType, eventType##_TYPE, dataType, signatureType >; \
         using Event          = typename Generator::Config::tEvent; \
         using Signature      = typename Generator::Config::tSignature; \
         using UserSignature  = typename Generator::Config::tUserSignature; \
         using Data           = typename Generator::Config::tData; \
         using Consumer       = typename Generator::Config::tConsumer; \
      };



   #define DEFINE_EVENT( eventType, dataType, signatureType ) \
      DEFINE_EVENT_S( eventType, dataType, signatureType )

   #define DEFINE_EVENT_SIMPLE( eventType, dataType ) \
      DEFINE_EVENT( eventType, dataType, carpc::async::simple::Signature )

   #define DEFINE_EVENT_ID( eventType, dataType, enumType ) \
      DEFINE_EVENT( eventType, dataType, carpc::async::id::TSignature< enumType > )

   #define DEFINE_IPC_EVENT( eventType, dataType, signatureType ) \
      DEFINE_IPC_EVENT_S( eventType, dataType, signatureType )

   #define DEFINE_IPC_EVENT_SIMPLE( eventType, dataType ) \
      DEFINE_IPC_EVENT( eventType, dataType, carpc::async::simple::Signature )

   #define DEFINE_IPC_EVENT_ID( eventType, dataType, enumType ) \
      DEFINE_IPC_EVENT( eventType, dataType, carpc::async::id::TSignature< enumType > )

#else

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
      DEFINE_EVENT_BASE( namespace, carpc::async::NoServiceType, eventType, dataType, signatureType )

   #define DEFINE_EVENT_SIMPLE_N( eventType, dataType ) \
      DEFINE_EVENT_N( eventType, dataType, carpc::async::simple::Signature )

   #define DEFINE_EVENT_ID_N( eventType, enumType ) \
      DEFINE_EVENT_N( eventType, dataType, carpc::async::id::TSignature< enumType > )

   #define DEFINE_IPC_EVENT_N( eventType, dataType, signatureType ) \
      DEFINE_EVENT_BASE( namespace, carpc::async::ServiceType, eventType, dataType, signatureType )

   #define DEFINE_IPC_EVENT_SIMPLE_N( eventType, dataType ) \
      DEFINE_IPC_EVENT_N( eventType, dataType, carpc::async::simple::Signature )

   #define DEFINE_IPC_EVENT_ID_N( eventType, enumType ) \
      DEFINE_IPC_EVENT_N( eventType, dataType, carpc::async::id::TSignature< enumType > )



   #define DEFINE_EVENT_S( eventType, dataType, signatureType ) \
      DEFINE_EVENT_BASE( struct, carpc::async::NoServiceType, eventType, dataType, signatureType )

   #define DEFINE_EVENT_SIMPLE_S( eventType, dataType ) \
      DEFINE_EVENT_S( eventType, dataType, carpc::async::simple::Signature )

   #define DEFINE_EVENT_ID_S( eventType, dataType, enumType ) \
      DEFINE_EVENT_S( eventType, dataType, carpc::async::id::TSignature< enumType > )

   #define DEFINE_IPC_EVENT_S( eventType, dataType, signatureType ) \
      DEFINE_EVENT_BASE( struct, carpc::async::ServiceType, eventType, dataType, signatureType )

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

#endif



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


















#if 0

#define _ARG_2_( _0, _1, _2, ... )  _2
#define _DETECT_TYPE_( ... )        _ARG_2_( __VA_ARGS__, SIMPLE_ID, SIMPLE )

/**********************************************************************************
 *
 * This DECLARE_EVENT macro allowes to declare event with user defined id type
 * or with default id type depending on third parameter.
 * If third parameter is empty will be defined event with default 'base::NoIdInfoType'
 * using DECLARE_EVENT_SIMPLE macro.
 * If third parameter is not empty will be defined event with user defined type
 * mentioned in third parameter using DECLARE_EVENT_SIMPLE_ID macro.
 *
 *********************************************************************************/
#define __DECLARE__(  TYPE, EVENT, DATA, ... )  DECLARE_EVENT_ ## TYPE ( EVENT, DATA, ##__VA_ARGS__ )
#define _DECLARE_(  TYPE, EVENT, DATA, ... )    __DECLARE__( TYPE, EVENT, DATA, ##__VA_ARGS__ )
#define DECLARE_EVENT( EVENT, DATA, ... )       _DECLARE_( _DETECT_TYPE_( DUMMY, ##__VA_ARGS__ ), EVENT, DATA, ##__VA_ARGS__ )

/**********************************************************************************
 *
 * This DECLARE_IPC_EVENT macro allowes to declare event with user defined id type
 * or with default id type depending on third parameter.
 * If third parameter is empty will be defined event with default 'base::NoIdInfoType'
 * using DECLARE_IPC_EVENT_SIMPLE macro.
 * If third parameter is not empty will be defined event with user defined type
 * mentioned in third parameter using DECLARE_IPC_EVENT_SIMPLE_ID macro.
 *
 *********************************************************************************/
#define __DECLARE_IPC__(  TYPE, EVENT, DATA, ... )  DECLARE_IPC_EVENT_ ## TYPE ( EVENT, DATA, ##__VA_ARGS__ )
#define _DECLARE_IPC_(  TYPE, EVENT, DATA, ... )   __DECLARE_IPC__( TYPE, EVENT, DATA, ##__VA_ARGS__ )
#define DECLARE_IPC_EVENT( EVENT, DATA, ... )      _DECLARE_IPC_( _DETECT_TYPE_( DUMMY, ##__VA_ARGS__ ), EVENT, DATA, ##__VA_ARGS__ )




/**********************************************************************************
 *
 * Declare simple event
 *
 *********************************************************************************/
#define DECLARE_EVENT_SIMPLE( eventType, dataType ) \
   namespace eventType { \
      class eventType; \
      using Event       = base::TGeneratorSimple< base::NoServiceType, eventType, dataType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

#define DECLARE_IPC_EVENT_SIMPLE( eventType, dataType ) \
   namespace eventType { \
      class ServiceType; \
      class eventType; \
      using Event       = base::TGeneratorSimple< ServiceType, eventType, dataType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

/**********************************************************************************
 *
 * Declare simple event with ID
 *
 *********************************************************************************/
#define DECLARE_EVENT_SIMPLE_ID( eventType, dataType, idType ) \
   namespace eventType { \
      class eventType; \
      using Event       = base::TGeneratorSimpleID< base::NoServiceType, eventType, dataType, idType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
      using ID          = idType; \
   }

#define DECLARE_IPC_EVENT_SIMPLE_ID( eventType, dataType, idType ) \
   namespace eventType { \
      class ServiceType; \
      class eventType; \
      using Event       = base::TGeneratorSimpleID< ServiceType, eventType, dataType, idType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
      using ID          = idType; \
   }

#endif
