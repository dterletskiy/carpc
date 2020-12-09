#pragma once

#include <set>
#include "imp/experimental/base/service/IService.hpp"



namespace fw::service {

   class IProxy;
   class Registry;



   class IStub
      : public std::enable_shared_from_this< IStub >
      , public IService
   {
         friend class Registry;

      private:
         enum class eRequestState :  std::uint8_t { BLOCKED, UNBLOCKED };

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
         IStub( const tServiceID&, const tServiceRole& );
         ~IStub( ) override;

      public:
         void check_in( ) override final;
         void check_out( ) override final;

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
         std::set< std::weak_ptr< IProxy >, Comparator > m_proxies;

      public:
         const tSeqID unblock_request( ) const;
         void client_seq_id( const tSeqID, std::weak_ptr< IProxy > );
      private:
         struct ProxySeqID
         {
            tSeqID seq_id;
            std::weak_ptr< IProxy > proxy;
         };
         std::map< tSeqID, ProxySeqID >   m_seq_id_mapping;
         tSeqID                           m_seq_id = 1000;
   };



   inline
   const tSeqID IStub::unblock_request( ) const
   {
      return m_seq_id;
   }

} // namespace fw::service
