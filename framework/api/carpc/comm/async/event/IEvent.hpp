#pragma once

#include "carpc/application/Context.hpp"
#include "carpc/comm/async/Types.hpp"
#include "carpc/comm/async/IAsync.hpp"



namespace carpc::application {

   class Context;

}

namespace carpc::async {

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

         struct TConsumer : public IAsync::IConsumer
         {
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

} // namespace carpc::async
