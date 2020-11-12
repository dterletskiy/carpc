#pragma once

#include <set>
#include "imp/experimental/base/service/IService.hpp"



namespace fw::service {

   class IProxy;

   class IStub : public IService
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

      public:
         IStub( );
         ~IStub( ) override;

         void connected( std::shared_ptr< IProxy > );
         void disconnected( std::shared_ptr< IProxy > );

         bool is_connected( ) const;
         bool is_disconnected( ) const;

      private:
         std::set< std::weak_ptr< IProxy >, Comparator > m_proxies;
   };

} // namespace fw::service
