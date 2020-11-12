#pragma once

#include <memory>
#include "imp/experimental/base/service/Types.hpp"



namespace fw::application {

   class Thread;

}

namespace fw::service {

   class IService : public std::enable_shared_from_this< IService >
   {
      public:
         IService( );
         virtual ~IService( );

      public:
         const tServiceID& service_id( ) const;
      private:
         tServiceID m_service_id;

      private:
         std::weak_ptr< application::Thread >   mp_thread;
   };

} // namespace fw::service
