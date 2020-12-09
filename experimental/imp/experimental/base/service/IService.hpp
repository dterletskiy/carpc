#pragma once

#include <memory>
#include "imp/experimental/base/application/Thread.hpp"
#include "imp/experimental/base/service/Types.hpp"



namespace fw::application {

   class Thread;

}

namespace fw::service {

   class IService
   {
      public:
         IService( const tServiceID&, const tServiceRole& );
         virtual ~IService( );

      public:
         virtual void check_in( ) = 0;
         virtual void check_out( ) = 0;

      private:
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;

      public:
         virtual bool is_connected( ) const = 0;
         virtual bool is_disconnected( ) const = 0;

      public:
         const Info& info( ) const;
         const tServiceID& id( ) const;
         const tServiceRole& role( ) const;
      private:
         Info           m_info;

      public:
         std::shared_ptr< application::Thread > thread( ) const;
      private:
         std::weak_ptr< application::Thread >   mp_thread;
   };



   inline
   const Info& IService::info( ) const
   {
      return m_info;
   }

   inline
   const tServiceID& IService::id( ) const
   {
      return m_info.id;
   }

   inline
   const tServiceRole& IService::role( ) const
   {
      return m_info.role;
   }

   inline
   std::shared_ptr< application::Thread > IService::thread( ) const
   {
      return mp_thread.lock( );
   }

} // namespace fw::service
