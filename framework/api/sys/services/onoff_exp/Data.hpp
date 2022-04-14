#pragma once

#include <memory>

#include "api/sys/comm/service/Types.hpp"
#include "api/sys/helpers/macros/strings.hpp"



namespace service::onoff {

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

} // namespace service::onoff



namespace service::onoff::ipc {

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
            virtual onoff::method::eID id( ) const = 0;
            virtual carpc::service::eType type( ) const = 0;
      };



      namespace request {

         class TriggerStateData : public BaseData
         {
            public:
               static constexpr onoff::method::eID ID = onoff::method::eID::TriggerState;

            public:
               TriggerStateData( ) = default;
               TriggerStateData( const std::string&, const std::size_t );
               ~TriggerStateData( ) override = default;

               std::string state = "";
               std::size_t delay = 0;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::method::eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::REQUEST; }
         };

      }

      namespace response {

         class TriggerStateData : public BaseData
         {
            public:
               static constexpr onoff::method::eID ID = onoff::method::eID::TriggerState;

            public:
               TriggerStateData( ) = default;
               TriggerStateData( const bool );
               ~TriggerStateData( ) override = default;

               bool result = false;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::method::eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::RESPONSE; }
         };
 
      }

      namespace request {

         class StartData : public BaseData
         {
            public:
               static constexpr onoff::method::eID ID = onoff::method::eID::Start;

            public:
               StartData( ) = default;
               ~StartData( ) override = default;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::method::eID id( ) const override { return ID; }
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
            virtual onoff::attribute::eID id( ) const = 0;
            virtual carpc::service::eType type( ) const = 0;
      };



      namespace notification {

         class CurrentStateData : public BaseData
         {
            public:
               static constexpr onoff::attribute::eID ID = onoff::attribute::eID::CurrentState;

            public:
               CurrentStateData( ) = default;
               CurrentStateData( const std::string& );
               ~CurrentStateData( ) override = default;

               std::string state = "";

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               onoff::attribute::eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::NOTIFICATION; }
         };

      }

   }



   struct Types
   {
      using tIPC           = carpc::service::IPC;

      struct method
      {
         using tID         = service::onoff::method::eID;
         using tBaseData   = service::onoff::ipc::method::BaseData;

         static bool has_response( const service::onoff::method::eID& request_id )
         {
            return service::onoff::method::has_response( request_id );
         }
      };

      struct attribute
      {
         using tID         = service::onoff::attribute::eID;
         using tBaseData   = service::onoff::ipc::attribute::BaseData;
      };
   };

} // namespace service::onoff::ipc



namespace service::onoff::no_ipc {

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
               static constexpr onoff::method::eID ID = onoff::method::eID::TriggerState;

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
               static constexpr onoff::method::eID ID = onoff::method::eID::TriggerState;

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
               static constexpr onoff::method::eID ID = onoff::method::eID::Start;

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
               static constexpr onoff::attribute::eID ID = onoff::attribute::eID::CurrentState;

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
         using tID         = service::onoff::method::eID;
         using tBaseData   = service::onoff::ipc::method::BaseData;
      };

      struct attribute
      {
         using tID         = service::onoff::attribute::eID;
         using tBaseData   = service::onoff::ipc::attribute::BaseData;
      };
   };

} // namespace service::onoff::no_ipc



namespace service::onoff {

   namespace data = service::onoff::ipc;

} // namespace service::onoff
