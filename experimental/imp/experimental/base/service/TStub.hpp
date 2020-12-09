#pragma once

#include <set>
#include "imp/experimental/base/service/IStub.hpp"



namespace fw::service {

   template< typename tProxy >
   class TStub : public IStub
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
         TStub( const tServiceID&, const tServiceRole& );
         ~TStub( ) override;

      private:
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      private:
         void connected( std::shared_ptr< IProxy > );
         void disconnected( std::shared_ptr< IProxy > );

      public:
         bool is_connected( ) const override;
         bool is_disconnected( ) const override;

      private:
         std::set< std::weak_ptr< tProxy >, Comparator > m_proxies;
   };



   template< typename tProxy >
   TStub< tProxy >::TStub( const tServiceID& id, const tServiceRole& role )
      : IProxy( id, role )
   {
   }

   template< typename tProxy >
   TStub< tProxy >::~TStub( )
   {
   }

} // namespace fw::service
