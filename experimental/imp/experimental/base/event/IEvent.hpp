#pragma once

#include <string>
#include <memory>
#include "imp/experimental/base/event/Types.hpp"



namespace fw::event {

   struct IEventConsumer : public std::enable_shared_from_this< IEventConsumer >
   {
      IEventConsumer( ) = default;
      virtual ~IEventConsumer( ) = default;
   };



   class IEvent
   {
      public:
         IEvent( ) = default;
         virtual ~IEvent( ) = default;

         static bool set_notification( std::shared_ptr< IEventConsumer >, const tClassID&, const tID& );
         static bool clear_notification( std::shared_ptr< IEventConsumer >, const tClassID&, const tID& );
         static bool send( std::shared_ptr< IEvent > );

         virtual bool send( ) = 0;
         virtual void process( std::shared_ptr< IEventConsumer > ) = 0;

      public:
         virtual const tClassID& class_id( ) const = 0;
         virtual const tID& id( ) const = 0;
   };

} // namespace fw::event
