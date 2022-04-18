#pragma once

#include "api/sys/helpers/macros/types.hpp"
#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/comm/service/experimental/TSignature.hpp"



namespace carpc::service::experimental {

   template< typename TYPES, typename Enable = void >
   struct TGenerator;



   #define ENABLE_IF_IPC_SERVICE CARPC_ENABLE_IF_IPC_TYPE( typename TYPES::tIPC )

   template< typename TYPES >
   struct TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >
   {
      // Methods (request/response) types definitions
      using T = TYPES;
      using tMethodBaseData      = typename TYPES::method::tBaseData;
      using tMethodBaseDataPtr   = std::shared_ptr< tMethodBaseData >;

      struct MethodData
      {
         MethodData( ) = default;
         MethodData( tMethodBaseDataPtr _ptr ) : ptr( _ptr ) { }

         tMethodBaseDataPtr ptr = nullptr;

         bool to_stream( carpc::ipc::tStream& stream ) const
         {
            if( nullptr == ptr ) return true;
            return ptr->serrialize( stream );
         }
         bool from_stream( carpc::ipc::tStream& stream )
         {
            ptr = tMethodBaseData::create( stream );
            return nullptr != ptr;
         }
      };
      DEFINE_IPC_EVENT_S( Method, MethodData, TSignature< typename TYPES::method::tID > );

      struct method
      {
         using tEvent               = typename Method::Event;
         using tEventConsumer       = typename Method::Consumer;
         using tEventUserSignature  = typename Method::UserSignature;
         using tEventID             = typename Method::UserSignature::tID; // => TYPES::method::tID
         using tEventData           = typename Method::Data; // => MethodData

         using tBaseData            = tMethodBaseData; // => TYPES::method::tBaseData
      };

      // Attributes types definitions
      using tAttributeBaseData      = typename TYPES::attribute::tBaseData;
      using tAttributeBaseDataPtr   = std::shared_ptr< tAttributeBaseData >;

      struct AttributeData
      {
         AttributeData( ) = default;
         AttributeData( tAttributeBaseDataPtr _ptr ) : ptr( _ptr ) { }

         tAttributeBaseDataPtr ptr = nullptr;

         bool to_stream( carpc::ipc::tStream& stream ) const
         {
            if( nullptr == ptr ) return true;
            return ptr->serrialize( stream );
         }
         bool from_stream( carpc::ipc::tStream& stream )
         {
            ptr = tAttributeBaseData::create( stream );
            return nullptr != ptr;
         }
      };
      DEFINE_IPC_EVENT_S( Attribute, AttributeData, TSignature< typename TYPES::attribute::tID > );

      struct attribute
      {
         using tEvent               = typename Attribute::Event;
         using tEventConsumer       = typename Attribute::Consumer;
         using tEventUserSignature  = typename Attribute::UserSignature;
         using tEventID             = typename Attribute::UserSignature::tID; // => TYPES::attribute::tID
         using tEventData           = typename Attribute::Data; // => AttributeData

         using tBaseData            = tAttributeBaseData; // => TYPES::attribute::tBaseData
      };

      static const async::tAsyncTypeID interface_type_id;
   };

   template< typename TYPES >
   const async::tAsyncTypeID
      TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >::interface_type_id =
         async::tAsyncTypeID::generate< TGenerator< TYPES, ENABLE_IF_IPC_SERVICE > >( );



   #define ENABVLE_IF_NO_IPC_SERVICE CARPC_ENABLE_IF_NO_IPC_TYPE( typename TYPES::tIPC )

   template< typename TYPES >
   struct TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >
   {
      // Methods (request/response) types definitions
      using tMethodBaseData      = typename TYPES::method::tBaseData;
      using tMethodBaseDataPtr   = std::shared_ptr< tMethodBaseData >;

      struct MethodData
      {
         MethodData( ) = default;
         MethodData( tMethodBaseDataPtr _ptr ) : ptr( _ptr ) { }

         tMethodBaseDataPtr ptr = nullptr;
      };
      DEFINE_IPC_EVENT_S( Method, MethodData, TSignature< typename TYPES::method::tID > );

      struct method
      {
         using tEvent               = typename Method::Event;
         using tEventConsumer       = typename Method::Consumer;
         using tEventUserSignature  = typename Method::UserSignature;
         using tEventID             = typename Method::UserSignature::tID; // => TYPES::method::tID
         using tEventData           = typename Method::Data; // => MethodData

         using tBaseData            = tMethodBaseData; // => TYPES::method::tBaseData
      };

      // Attributes types definitions
      using tAttributeBaseData      = typename TYPES::attribute::tBaseData;
      using tAttributeBaseDataPtr   = std::shared_ptr< tAttributeBaseData >;

      struct AttributeData
      {
         AttributeData( ) = default;
         AttributeData( tAttributeBaseDataPtr _ptr ) : ptr( _ptr ) { }

         tAttributeBaseDataPtr ptr = nullptr;
      };
      DEFINE_IPC_EVENT_S( Attribute, AttributeData, TSignature< typename TYPES::attribute::tID > );

      struct attribute
      {
         using tEvent               = typename Attribute::Event;
         using tEventConsumer       = typename Attribute::Consumer;
         using tEventUserSignature  = typename Attribute::UserSignature;
         using tEventID             = typename Attribute::UserSignature::tID; // => TYPES::attribute::tID
         using tEventData           = typename Attribute::Data; // => AttributeData

         using tBaseData            = tAttributeBaseData; // => TYPES::attribute::tBaseData
      };

      static const async::tAsyncTypeID interface_type_id;
   };

   template< typename TYPES >
   const async::tAsyncTypeID
      TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >::interface_type_id =
         async::tAsyncTypeID::generate< TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE > >( );

} // namespace carpc::service::experimental
