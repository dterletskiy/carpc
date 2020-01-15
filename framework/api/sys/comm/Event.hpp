#pragma once

#include "api/sys/comm/TGenerator.hpp"
#include "api/sys/comm/EventRegistry.hpp"
#include "api/sys/comm/EventCreator.hpp"
#include "api/sys/helpers/macros/enum.hpp"



#define _ARG_2_( _0, _1, _2, ... )  _2
#define _DETECT_TYPE_( ... )        _ARG_2_( __VA_ARGS__, USER_ID, DEFAULT_ID )

/**********************************************************************************
 *
 * This DECLARE_EVENT macro allowes to declare event with user defined id type
 * or with default id type depending on third parameter.
 * If third parameter is empty will be defined event with default 'base::NoIdInfoType'
 * using DECLARE_EVENT_DEFAULT_ID macro.
 * If third parameter is not empty will be defined event with user defined type
 * mentioned in third parameter using DECLARE_EVENT_USER_ID macro.
 *
 *********************************************************************************/
#define __DECLARE__(  TYPE, EVENT, DATA, ... )  DECLARE_EVENT_ ## TYPE ( EVENT, DATA, ##__VA_ARGS__ )
#define _DECLARE_(  TYPE, EVENT, DATA, ... )    __DECLARE__( TYPE, EVENT, DATA, ##__VA_ARGS__ )
#define DECLARE_EVENT( EVENT, DATA, ... )       _DECLARE_( _DETECT_TYPE_( DUMMY, ##__VA_ARGS__ ), EVENT, DATA, ##__VA_ARGS__ )



#define DECLARE_EVENT_DEFAULT_ID( eventType, dataType ) \
   namespace eventType { \
      class eventType; \
      using Event       = base::TGenerator< dataType, base::NoIdInfoType, eventType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

#define DECLARE_EVENT_USER_ID( eventType, dataType, idType ) \
   namespace eventType { \
      class eventType; \
      using Event       = base::TGenerator< dataType, idType, eventType >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

#define INIT_EVENT( eventType ) \
   template< > base::EventTypeID eventType::Event::_TEventBase::s_type_id = { #eventType };

#define REGISTER_EVENT( eventType ) \
   base::EventRegistry::instance( )->register_event( #eventType, base::create_event< eventType::Event > );



/**********************************************************************************
 *
 * This DECLARE_DSI_EVENT macro allowes to declare event with user defined id type
 * or with default id type depending on third parameter.
 * If third parameter is empty will be defined event with default 'base::NoIdInfoType'
 * using DECLARE_DSI_EVENT_DEFAULT_ID macro.
 * If third parameter is not empty will be defined event with user defined type
 * mentioned in third parameter using DECLARE_DSI_EVENT_USER_ID macro.
 *
 *********************************************************************************/
#define __DECLARE_DSI__(  TYPE, SERVICE, EVENT, DATA, ... )  DECLARE_DSI_EVENT_ ## TYPE ( SERVICE, EVENT, DATA, ##__VA_ARGS__ )
#define _DECLARE_DSI_(  TYPE, SERVICE, EVENT, DATA, ... )   __DECLARE_DSI__( TYPE, SERVICE, EVENT, DATA, ##__VA_ARGS__ )
#define DECLARE_DSI_EVENT( SERVICE, EVENT, DATA, ... )      _DECLARE_DSI_( _DETECT_TYPE_( DUMMY, ##__VA_ARGS__ ), SERVICE, EVENT, DATA, ##__VA_ARGS__ )

#define DECLARE_DSI_EVENT_DEFAULT_ID( serviceName, eventType, dataType ) \
   namespace serviceName::eventType { \
      class serviceName; \
      class eventType; \
      using Event       = base::TGenerator< dataType, base::NoIdInfoType, eventType, serviceName >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

#define DECLARE_DSI_EVENT_USER_ID( serviceName, eventType, dataType, idType ) \
   namespace serviceName::eventType { \
      class serviceName; \
      class eventType; \
      using Event       = base::TGenerator< dataType, idType, eventType, serviceName >::Config::EventType; \
      using Consumer    = Event::_ConsumerType; \
      using Data        = dataType; \
   }

#define INIT_DSI_EVENT( serviceName, eventType ) \
      template< > base::EventTypeID serviceName::eventType::Event::_TEventBase::s_type_id = { #eventType"."#serviceName };

#define REGISTER_DSI_EVENT( serviceName, eventType ) \
   base::EventRegistry::instance( )->register_event( #eventType"."#serviceName, base::create_event< serviceName::eventType::Event > );


