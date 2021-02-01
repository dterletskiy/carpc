#pragma once

#include "api/sys/comm/async/event/Event.hpp"
#include "api/sys/comm/service/fast/TSignature.hpp"



namespace base::service::fast {

   template< typename TYPES, typename Enable = void >
   struct TGenerator;



   #define IS_IPC_SERVICE std::is_same_v< typename TYPES::tIPC, IPC >
   #define ENABLE_IF_IPC_SERVICE std::enable_if_t< IS_IPC_SERVICE >

   template< typename TYPES >
   struct TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >
   {
      using tBaseData      = typename TYPES::tBaseData;
      using tBaseDataPtr = std::shared_ptr< tBaseData >;

      struct ServiceData
      {
         ServiceData( ) = default;
         ServiceData( tBaseDataPtr _ptr ) : ptr( _ptr ) { }

         tBaseDataPtr ptr = nullptr;

         bool to_stream( base::dsi::tByteStream& stream ) const
         {
            if( nullptr == ptr ) return true;
            return ptr->serrialize( stream );
         }
         bool from_stream( base::dsi::tByteStream& stream )
         {
            ptr = tBaseData::create( stream );
            return nullptr != ptr;
         }
      };
      DEFINE_IPC_EVENT_S( Service, ServiceData, TSignature< typename TYPES::tID > );

      using tEvent               = typename Service::Event;
      using tEventConsumer       = typename Service::Consumer;
      using tEventUserSignature  = typename Service::UserSignature;
      using tEventID             = typename Service::UserSignature::tID;
      using tEventData           = typename Service::Data;

      static const typename RequestResponseIDs< tEventID >::tVector& RR;
      static const typename NotificationIDs< tEventID >::tVector& N;
      static const async::tAsyncTypeID interface_type_id;
   };

   template< typename TYPES >
   const async::tAsyncTypeID
      TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >::interface_type_id =
         TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >::Service::Signature::build_type_id( );

   template< typename TYPES >
   const typename RequestResponseIDs< typename TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >::tEventID >::tVector&
      TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >::RR =
         TYPES::RR;

   template< typename TYPES >
   const typename NotificationIDs< typename TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >::tEventID >::tVector&
      TGenerator< TYPES, ENABLE_IF_IPC_SERVICE >::N =
         TYPES::N;



   #define IS_NO_IPC_SERVICE std::is_same_v< typename TYPES::tIPC, NO_IPC >
   #define ENABVLE_IF_NO_IPC_SERVICE std::enable_if_t< IS_NO_IPC_SERVICE >

   template< typename TYPES >
   struct TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >
   {
      using tBaseData      = typename TYPES::tBaseData;
      using tBaseDataPtr = std::shared_ptr< tBaseData >;

      struct ServiceData
      {
         ServiceData( ) = default;
         ServiceData( tBaseDataPtr _ptr ) : ptr( _ptr ) { }

         tBaseDataPtr ptr = nullptr;
      };
      DEFINE_EVENT_S( Service, ServiceData, TSignature< typename TYPES::tID > );

      using tEvent               = typename Service::Event;
      using tEventConsumer       = typename Service::Consumer;
      using tEventUserSignature  = typename Service::UserSignature;
      using tEventID             = typename Service::UserSignature::tID;
      using tEventData           = typename Service::Data;

      static const typename RequestResponseIDs< tEventID >::tVector& RR;
      static const typename NotificationIDs< tEventID >::tVector& N;
      static const async::tAsyncTypeID interface_type_id;
   };

   template< typename TYPES >
   const async::tAsyncTypeID
      TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >::interface_type_id =
         TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >::Service::Signature::build_type_id( );

   template< typename TYPES >
   const typename RequestResponseIDs< typename TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >::tEventID >::tVector&
      TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >::RR =
         TYPES::RR;

   template< typename TYPES >
   const typename NotificationIDs< typename TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >::tEventID >::tVector&
      TGenerator< TYPES, ENABVLE_IF_NO_IPC_SERVICE >::N =
         TYPES::N;

} // namespace base::service::fast
