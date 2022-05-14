#pragma once

#include <memory>
#include "api/sys/helpers/macros/strings.hpp"
#include "api/service/onoff/2.0.0/Version.hpp"

namespace service::onoff::V2_0_0 {

   namespace method {

      DEFINE_ENUMERATION( eID, std::uint8_t
         , trigger_state
         , start
      );

      inline bool has_response( const eID& request_id )
      {
         return false
            || eID::trigger_state == request_id
         ;
      }

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
            virtual eID id( ) const = 0;
            virtual carpc::service::eType type( ) const = 0;
      };

      namespace request {

         class trigger_stateData : public BaseData
         {
            public:
               static constexpr eID ID = eID::trigger_state;

            public:
               trigger_stateData( ) = default;
               trigger_stateData( const std::string& _state, const std::size_t& _timeout );
               ~trigger_stateData( ) override = default;

               std::string state;
               std::size_t timeout;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::REQUEST; }

         };

      } // namespace request

      namespace response {

         class trigger_stateData : public BaseData
         {
            public:
               static constexpr eID ID = eID::trigger_state;

            public:
               trigger_stateData( ) = default;
               trigger_stateData( const bool& _result );
               ~trigger_stateData( ) override = default;

               bool result;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::RESPONSE; }

         };

      } // namespace response

      namespace request {

         class startData : public BaseData
         {
            public:
               static constexpr eID ID = eID::start;

            public:
               startData( ) = default;
               ~startData( ) override = default;


            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::REQUEST; }

         };

      } // namespace request

   } // namespace method

   namespace attribute {

      DEFINE_ENUMERATION( eID, std::uint8_t
         , current_state
      );

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
            virtual eID id( ) const = 0;
            virtual carpc::service::eType type( ) const = 0;
      };

      namespace notification {

         class current_stateData : public BaseData
         {
            public:
               static constexpr eID ID = eID::current_state;

            public:
               current_stateData( ) = default;
               current_stateData( const std::string& _value );
               ~current_stateData( ) override = default;

               std::string value;

            private:
               bool to_stream( carpc::ipc::tStream& ) override;
               bool from_stream( carpc::ipc::tStream& ) override;
               eID id( ) const override { return ID; }
               carpc::service::eType type( ) const override { return carpc::service::eType::NOTIFICATION; }

         };

      } // namespace notification

   } // namespace attribute

   struct Types
   {
      using tIPC           = carpc::IPC;

      struct method
      {
         using tID         = V2_0_0::method::eID;
         using tBaseData   = V2_0_0::method::BaseData;

         static bool has_response( const V2_0_0::method::eID& request_id )
         {
            return V2_0_0::method::has_response( request_id );
         }
      };

      struct attribute
      {
         using tID         = V2_0_0::attribute::eID;
         using tBaseData   = V2_0_0::attribute::BaseData;
      };
   };

} // namespace service::onoff::V2_0_0


