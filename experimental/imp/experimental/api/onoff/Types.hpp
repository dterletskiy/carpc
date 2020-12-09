#pragma once

#include <cstring>
#include "imp/experimental/base/event/Event.hpp"
#include "imp/experimental/base/service/Types.hpp"



namespace api::onoff {

   enum class eEventID : fw::event::tID
   {
      RequestBoot,
      ResponseBoot,
      RequestBootBusy
   };
   const fw::event::tID convert( const eEventID );

   struct BaseData
   {
   };

   struct RequestBootData : public BaseData
   {
      RequestBootData( const std::string& msg ) : message( msg ) { }
      std::string message;
   };
   struct ResponseBootData : public BaseData
   {
      ResponseBootData( const bool rst ) : result( rst ) { }
      bool result;
   };

   struct EventData
   {
      std::shared_ptr< BaseData > mp_data = nullptr;
   };
   DECLARE_EVENT( Event, EventData, IEventConsumer );

   class Service{ };
   const fw::service::tServiceID ServiceID = typeid( Service ).name( );

} // namespace api::onoff
