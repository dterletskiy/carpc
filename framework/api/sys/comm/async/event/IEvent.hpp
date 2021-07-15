#pragma once

#include "api/sys/application/Context.hpp"
#include "api/sys/comm/async/Types.hpp"
#include "api/sys/comm/async/IAsync.hpp"



namespace base::application {

   class Context;

}

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
         };

         template< typename _EventType >
         struct TConsumer : public IAsync::IConsumer
         {
            public:
               TConsumer( ) = default;
               ~TConsumer( ) override { _EventType::clear_all_notifications( this ); }

               virtual void process_event( const _EventType& ) = 0;
         };

      public:
         IEvent( ) = default;
         ~IEvent( ) override = default;

      public:
         static const bool set_notification( IAsync::IConsumer*, const ISignature::tSptr );
         static const bool clear_notification( IAsync::IConsumer*, const ISignature::tSptr );
         static const bool clear_all_notifications( IAsync::IConsumer*, const ISignature::tSptr );

         static bool check_in( const tAsyncTypeID&, tCreator );
         static void dump( );
         static bool serialize( ipc::tStream&, IEvent::tSptr );
         static bool serialize( ipc::tStream&, const IEvent& );
         static tSptr deserialize( ipc::tStream& );

      public:
         const bool send( const application::Context& to_context = application::Context::internal_broadcast );

      private:
         void process( IAsync::IConsumer* ) const override;
         virtual void process_event( IAsync::IConsumer* ) const = 0;
         const eAsyncType type( ) const override final;

      public:
         const bool to_stream( ipc::tStream& ) const;
         const bool from_stream( ipc::tStream& );
      private:
         virtual const bool to_stream_t( ipc::tStream& ) const = 0;
         virtual const bool from_stream_t( ipc::tStream& ) = 0;

      public:
         virtual const bool is_ipc( ) const = 0;
         virtual const application::Context& context( ) const = 0;
   };



   inline
   const eAsyncType IEvent::type( ) const
   {
      return eAsyncType::EVENT;
   }

} // namespace base::async
