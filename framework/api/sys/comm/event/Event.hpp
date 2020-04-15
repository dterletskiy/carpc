#pragma once

#include "api/sys/comm/event/TEvent.hpp"
#include "api/sys/comm/event/EventRegistry.hpp"
#include "api/sys/helpers/macros/enum.hpp"



#define DEFINE_EVENT( eventType, dataType, signatureType ) \
   namespace eventType { \
      class eventType; \
      using Generator      = base::TGenerator< base::NoServiceType, eventType, dataType, signatureType >; \
      using Event          = Generator::Config::tEvent; \
      using Signature      = Generator::Config::tSignature; \
      using UserSignature  = Generator::Config::tUserSignature; \
      using Data           = dataType; \
      using Consumer       = Generator::Config::tConsumer; \
   }

#define DEFINE_IPC_EVENT( eventType, dataType, signatureType ) \
   namespace eventType { \
      class ServiceType; \
      class eventType; \
      using Generator      = base::TGenerator< ServiceType, eventType, dataType, signatureType >; \
      using Event          = Generator::Config::tEvent; \
      using Signature      = Generator::Config::tSignature; \
      using UserSignature  = Generator::Config::tUserSignature; \
      using Data           = dataType; \
      using Consumer       = Generator::Config::tConsumer; \
   }

#define REGISTER_EVENT( eventType ) \
   base::EventRegistry::instance( )->register_event( base::build_type_id< eventType::Signature >( ), base::create_event< eventType::Event > );



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

/**********************************************************************************
 *
 * Declare Request/Response event
 *
 *********************************************************************************/
#define DECLARE_EVENT_RR( eventType, dataType, idType ) \
   namespace eventType { \
      class eventType; \
      using Event       = base::TGeneratorRR< base::NoServiceType, eventType, dataType, idType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
      using ID          = idType; \
      using Signature   = Event::_Signature; \
   }

#define DECLARE_IPC_EVENT_RR( eventType, dataType, idType ) \
   namespace eventType { \
      class ServiceType; \
      class eventType; \
      using Event       = base::TGeneratorRR< ServiceType, eventType, dataType, idType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
      using ID          = idType; \
      using Signature   = Event::_Signature; \
   }



/**********************************************************************************
 *
 * This INIT_EVENT macro initializes static member for all event types.
 *
 *********************************************************************************/
#define INIT_EVENT( eventType ) \
   template< > const base::EventTypeID eventType::Event::_TEventBase::Signature::s_type_id = { #eventType }; \
   template< > const base::eEventType eventType::Event::_TEventBase::Signature::s_type_event = eventType::Event::_Signature::build_type( );

/**********************************************************************************
 *
 * This REGISTER_EVENT macro registers all event types.
 *
 *********************************************************************************/
#define REGISTER_EVENT( eventType ) \
   base::EventRegistry::instance( )->register_event( #eventType, base::create_event< eventType::Event > );

#endif
