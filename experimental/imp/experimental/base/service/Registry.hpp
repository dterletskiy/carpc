#pragma once

#include <list>
#include <map>
#include <memory>
#include "imp/experimental/base/service/IStub.hpp"
#include "imp/experimental/base/service/IProxy.hpp"



namespace fw::service {

   class Registry
   {
      private:
         struct Connection
         {
            std::weak_ptr< IStub >                 stub;
            std::list< std::weak_ptr< IProxy > >   proxies;
         };

      public:
         ~Registry( );
      private:
         Registry( );
         Registry( const Registry& ) = delete;
         Registry& operator=( const Registry& ) = delete;

      public:
         static Registry& instance( );
      private:
         static std::unique_ptr< Registry > mp_instance;

      public:
         void register_stub( std::shared_ptr< IStub > );
         void unregister_stub( std::shared_ptr< IStub > );
         void register_proxy( std::shared_ptr< IProxy > );
         void unregister_proxy( std::shared_ptr< IProxy > );
      private:

      private:
         std::map< tServiceID, Connection >     m_registry;

      private:
         std::weak_ptr< application::Thread >   mp_thread;
   };

} // namespace fw::service
