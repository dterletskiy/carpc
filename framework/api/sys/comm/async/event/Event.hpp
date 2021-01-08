#pragma once

#include "api/sys/comm/async/event/TGenerator.hpp"
#include "api/sys/helpers/macros/enum.hpp"



#define DEFINE_EVENT_N( eventType, dataType, signatureType ) \
   namespace eventType { \
      class eventType; \
      using Generator      = base::async::TGenerator< base::async::NoServiceType, eventType, dataType, signatureType >; \
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
      using Generator      = base::async::TGenerator< ServiceType, eventType, dataType, signatureType >; \
      using Event          = Generator::Config::tEvent; \
      using Signature      = Generator::Config::tSignature; \
      using UserSignature  = Generator::Config::tUserSignature; \
      using Data           = Generator::Config::tData; \
      using Consumer       = Generator::Config::tConsumer; \
   }



#define DEFINE_EVENT_S( eventType, dataType, signatureType ) \
   struct eventType { \
      class eventType##_TYPE; \
      using Generator      = base::async::TGenerator< base::async::NoServiceType, eventType##_TYPE, dataType, signatureType >; \
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
      using Generator      = base::async::TGenerator< ServiceType, eventType##_TYPE, dataType, signatureType >; \
      using Event          = typename Generator::Config::tEvent; \
      using Signature      = typename Generator::Config::tSignature; \
      using UserSignature  = typename Generator::Config::tUserSignature; \
      using Data           = typename Generator::Config::tData; \
      using Consumer       = typename Generator::Config::tConsumer; \
   };



#define DEFINE_EVENT( eventType, dataType, signatureType ) DEFINE_EVENT_N( eventType, dataType, signatureType )

#define DEFINE_IPC_EVENT( eventType, dataType, signatureType ) DEFINE_IPC_EVENT_N( eventType, dataType, signatureType )



#define REGISTER_EVENT( eventType ) \
   base::async::IEvent::check_in( eventType::Signature::build_type_id( ), eventType::Event::create );

#define DUMP_IPC_EVENTS \
   base::async::IEvent::dump( );
