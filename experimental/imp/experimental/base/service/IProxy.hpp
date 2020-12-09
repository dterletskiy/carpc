#pragma once

#include "imp/experimental/base/service/IService.hpp"



namespace fw::service {

   class IStub;
   class Registry;



   class IProxy
      : public std::enable_shared_from_this< IProxy >
      , public IService
   {
         friend class Registry;

      public:
         IProxy( const tServiceID&, const tServiceRole& );
         ~IProxy( ) override;

      public:
         void check_in( ) override final;
         void check_out( ) override final;

      private:
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      private:
         void connected( std::shared_ptr< IStub > );
         void disconnected( std::shared_ptr< IStub > );

      public:
         bool is_connected( ) const override;
         bool is_disconnected( ) const override;

      protected:
         void request( application::Context::tFunction );
         template< typename T >
            std::shared_ptr< T > stub( ) const;
      private:
         std::weak_ptr< IStub >  mp_stub;
         tSeqID                  m_seq_id = 0;
   };



   template< typename T >
   std::shared_ptr< T > IProxy::stub( ) const
   {
      return std::static_pointer_cast< T >( mp_stub.lock( ) );
   }

} // namespace fw::service
