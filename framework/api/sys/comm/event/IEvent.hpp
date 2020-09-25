#pragma once

#include "api/sys/service/IServiceThread.hpp"
#include "api/sys/comm/event/Types.hpp"
#include "api/sys/comm/event/IAsync.hpp"



namespace base::async {

   class IEvent
      : public IAsync
      , public std::enable_shared_from_this< IEvent >
   {
      public:
         using tSptr = std::shared_ptr< IEvent >;
         using tCreator = IEvent::tSptr(*)( );

      public:
         struct ISignature : public IAsync::ISignature
         {
            const eAsyncType type( ) const override final { return eAsyncType::EVENT; }

            virtual const bool to_stream( dsi::tByteStream& ) const = 0;
            virtual const bool from_stream( dsi::tByteStream& ) = 0;
         };

         template< typename _EventType >
         struct TConsumer : public IAsync::IConsumer
         {
            TConsumer( ) = default;
            ~TConsumer( ) override { _EventType::clear_all_notifications( this ); }

            virtual void process_event( const _EventType& ) = 0;
         };

      public:
         IEvent( const eCommType );
         ~IEvent( ) override = default;

      public:
         static const bool set_notification( IAsync::IConsumer*, const ISignature& );
         static const bool clear_notification( IAsync::IConsumer*, const ISignature& );
         static const bool clear_all_notifications( IAsync::IConsumer*, const ISignature& );
         static const bool send( tSptr, const eCommType comm_type = eCommType::NONE );
         static const bool send_to_context( tSptr, IServiceThread::tWptr );

         static bool check_in( const tAsyncTypeID&, tCreator );
         static void dump( );
         static bool serialize( dsi::tByteStream&, IEvent::tSptr );
         static bool serialize( dsi::tByteStream&, const IEvent& );
         static tSptr deserialize( dsi::tByteStream& );

      public:
         virtual const bool send( const eCommType comm_type = eCommType::NONE ) = 0;
         virtual const bool send_to_context( IServiceThread::tWptr ) = 0;

      private:
         void process( IAsync::IConsumer* ) const override;
         virtual void process_event( IAsync::IConsumer* ) const = 0;

      public:
         // serialization / deserialization methods should
         // serialize / deserialize all data exapt of type_id and communication type
         virtual const bool to_stream( dsi::tByteStream& ) const = 0;
         virtual const bool from_stream( dsi::tByteStream& ) = 0;

      public:
         virtual const bool is_ipc( ) const = 0;

      public:
         const eCommType& comm_type( );
      private:
         eCommType m_comm_type;
   };



   inline
   IEvent::IEvent( const eCommType comm_type )
      : m_comm_type( comm_type )
   {
   }

   inline
   const eCommType& IEvent::comm_type( )
   {
      if( eCommType::NONE == m_comm_type )
         m_comm_type = is_ipc( ) ? eCommType::IPC : eCommType::ITC;

      return m_comm_type;
   }



} // namespace base::async
