#pragma once

#include <set>
#include <map>
#include <memory>
#include "imp/experimental/base/service/IService.hpp"



namespace fw::service {

   class IStub;
   class IProxy;



   class Registry
   {
      private:
         struct Comparator
         {
            bool operator( )( const std::weak_ptr< IProxy > wp_1, const std::weak_ptr< IProxy > wp_2 ) const
            {
               auto sp_1 = wp_1.lock( );
               auto sp_2 = wp_2.lock( );
               if( !sp_1 || !sp_2 )
                  return false;
               return sp_1 < sp_2;
            }
         };

         struct Connection
         {
            std::weak_ptr< IStub >                             stub;
            std::set< std::weak_ptr< IProxy >, Comparator >    proxies;
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
         void check_in( std::shared_ptr< IStub > );
         void check_out( std::shared_ptr< IStub > );
         void check_in( std::shared_ptr< IProxy > );
         void check_out( std::shared_ptr< IProxy > );
      private:
         void register_stub( std::shared_ptr< IStub > );
         void unregister_stub( std::shared_ptr< IStub > );
         void register_proxy( std::shared_ptr< IProxy > );
         void unregister_proxy( std::shared_ptr< IProxy > );

      private:
         std::map< Info, Connection >     m_registry;

      private:
         std::weak_ptr< application::Thread >   mp_thread;
   };

} // namespace fw::service
