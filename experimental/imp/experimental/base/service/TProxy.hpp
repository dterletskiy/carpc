#pragma once

#include "imp/experimental/base/service/IProxy.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TProxy"



namespace fw::service {

   template< typename tStub >
   class TProxy : public IProxy
   {
         friend class Registry;

      public:
         TProxy( const tServiceID&, const tServiceRole& );
         ~TProxy( ) override;

      private:
         void connected( ) override = 0;
         void disconnected( ) override = 0;

      private:
         void connected( std::shared_ptr< IStub > ) override;
         void disconnected( std::shared_ptr< IStub > ) override;

      public:
         bool is_connected( ) const override;
         bool is_disconnected( ) const override;

      protected:
         std::shared_ptr< tStub > stub( ) const;
      private:
         std::weak_ptr< tStub > mp_stub;
   };



   template< typename tStub >
   TProxy< tStub >::TProxy( const tServiceID& id, const tServiceRole& role )
      : IProxy( id, role )
   {
   }

   template< typename tStub >
   TProxy< tStub >::~TProxy( )
   {
   }

   template< typename tStub >
   void TProxy< tStub >::connected( std::shared_ptr< IStub > p_stub )
   {
      if( auto sp_thread = thread( ) )
      {
         auto function = [ this, p_stub ]( )
         {
            mp_stub = std::static_pointer_cast< tStub >( p_stub );
            connected( );
         };
         sp_thread->post( function );
      }
      else
      {
         DBG_WRN( "current context is not exists" );
      }
   }

   template< typename tStub >
   void TProxy< tStub >::disconnected( std::shared_ptr< IStub > p_stub )
   {
      if( auto sp_thread = thread( ) )
      {
         auto function = [ this ]( )
         {
            mp_stub.reset( );
            disconnected( );
         };
         sp_thread->post( function );
      }
      else
      {
         DBG_WRN( "current context is not exists" );
      }
   }

   template< typename tStub >
   bool TProxy< tStub >::is_connected( ) const
   {
      return !mp_stub.expired( );
   }

   template< typename tStub >
   bool TProxy< tStub >::is_disconnected( ) const
   {
      return is_connected( );
   }

   template< typename tStub >
   std::shared_ptr< tStub > TProxy< tStub >::stub( ) const
   {
      return mp_stub.lock( );
   }

} // namespace fw::service



#undef CLASS_ABBR
