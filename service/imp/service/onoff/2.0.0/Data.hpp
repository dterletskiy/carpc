#pragma once

#include <memory>

#include "api/sys/comm/service/Types.hpp"
#include "api/sys/helpers/macros/strings.hpp"
#include "api/service/onoff/2.0.0/Version.hpp"



namespace service::onoff::V2_0_0 {

   namespace method {

      DEFINE_ENUMERATION( eID, std::uint8_t
         , TriggerState
         , Start
         , Undefined // Must present always for all interfaces
      );

      inline
      bool has_response( const eID& request_id )
      {
         return eID::TriggerState == request_id;
      }

   }

   namespace attribute {

      DEFINE_ENUMERATION( eID, std::uint8_t
         , CurrentState
         , Undefined // Must present always for all interfaces
      );

   }

} // namespace service::onoff::V2_0_0



namespace service::onoff::V2_0_0::ipc {

   namespace method {

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
            virtual onoff::V2_0_0::method::eID id( ) const = 0;
            virtual carpc::service::eType type( ) const = 0;
      };



      namespace request {

         class TriggerStateData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::method::eID ID = onoff::V2_0_0::method::eID::TriggerState;

            public:
               TriggerStateData( ) = default;
               TriggerStateData( const std::string&, const std::size_t );
               ~TriggerStateData( ) override = default;

               std::string state = "";
               std::size_t delay = 0;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::V2_0_0::method::eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::REQUEST; }
         };

      }

      namespace response {

         class TriggerStateData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::method::eID ID = onoff::V2_0_0::method::eID::TriggerState;

            public:
               TriggerStateData( ) = default;
               TriggerStateData( const bool );
               ~TriggerStateData( ) override = default;

               bool result = false;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::V2_0_0::method::eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::RESPONSE; }
         };
 
      }

      namespace request {

         class StartData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::method::eID ID = onoff::V2_0_0::method::eID::Start;

            public:
               StartData( ) = default;
               ~StartData( ) override = default;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::V2_0_0::method::eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::REQUEST; }
         };

      }

   }



   namespace attribute {

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
            virtual onoff::V2_0_0::attribute::eID id( ) const = 0;
            virtual carpc::service::eType type( ) const = 0;
      };



      namespace notification {

         class CurrentStateData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::attribute::eID ID = onoff::V2_0_0::attribute::eID::CurrentState;

            public:
               CurrentStateData( ) = default;
               CurrentStateData( const std::string& );
               ~CurrentStateData( ) override = default;

               std::string state = "";

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::V2_0_0::attribute::eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::NOTIFICATION; }
         };

      }

   }



   struct Types
   {
      using tIPC           = carpc::service::IPC;

      struct method
      {
         using tID         = service::onoff::V2_0_0::method::eID;
         using tBaseData   = service::onoff::V2_0_0::ipc::method::BaseData;

         static bool has_response( const service::onoff::V2_0_0::method::eID& request_id )
         {
            return service::onoff::V2_0_0::method::has_response( request_id );
         }
      };

      struct attribute
      {
         using tID         = service::onoff::V2_0_0::attribute::eID;
         using tBaseData   = service::onoff::V2_0_0::ipc::attribute::BaseData;
      };
   };

} // namespace service::onoff::V2_0_0::ipc



namespace service::onoff::V2_0_0::no_ipc {

   namespace method {

      class BaseData
      {
         public:
            using tSptr = std::shared_ptr< BaseData >;

            BaseData( ) = default;
            virtual ~BaseData( ) = default;
      };



      namespace request {

         class TriggerStateData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::method::eID ID = onoff::V2_0_0::method::eID::TriggerState;

            public:
               TriggerStateData( ) = default;
               TriggerStateData( const std::string&, const std::size_t );
               ~TriggerStateData( ) override = default;

               std::string state = "";
               std::size_t delay = 0;
         };

      }

      namespace response {

         class TriggerStateData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::method::eID ID = onoff::V2_0_0::method::eID::TriggerState;

            public:
               TriggerStateData( ) = default;
               TriggerStateData( const bool );
               ~TriggerStateData( ) override = default;

               bool result = false;
         };
 
      }

      namespace request {

         class StartData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::method::eID ID = onoff::V2_0_0::method::eID::Start;

            public:
               StartData( ) = default;
               ~StartData( ) override = default;
         };

      }

   }



   namespace attribute {

      class BaseData
      {
         public:
            using tSptr = std::shared_ptr< BaseData >;

            BaseData( ) = default;
            virtual ~BaseData( ) = default;
      };



      namespace notification {

         class CurrentStateData : public BaseData
         {
            public:
               static constexpr onoff::V2_0_0::attribute::eID ID = onoff::V2_0_0::attribute::eID::CurrentState;

            public:
               CurrentStateData( ) = default;
               CurrentStateData( const std::string& );
               ~CurrentStateData( ) override = default;

               std::string state = "";
         };

      }

   }



   struct Types
   {
      using tIPC           = carpc::service::NO_IPC;

      struct method
      {
         using tID         = service::onoff::V2_0_0::method::eID;
         using tBaseData   = service::onoff::V2_0_0::ipc::method::BaseData;
      };

      struct attribute
      {
         using tID         = service::onoff::V2_0_0::attribute::eID;
         using tBaseData   = service::onoff::V2_0_0::ipc::attribute::BaseData;
      };
   };

} // namespace service::onoff::V2_0_0::no_ipc



namespace service::onoff::V2_0_0 {

   namespace data = service::onoff::V2_0_0::ipc;

} // namespace service::onoff::V2_0_0
