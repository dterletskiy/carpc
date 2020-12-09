#pragma once

#include "imp/experimental/base/event/IEvent.hpp"



namespace fw::event {

   template< typename _Common >
   struct TEventConsumer : public IEventConsumer
   {
      TEventConsumer( ) = default;
      ~TEventConsumer( ) override = default;

      virtual void process( const typename _Common::tEvent& ) = 0;
   };



   template< typename _Common >
   class TEvent
      : public IEvent
      , public std::enable_shared_from_this< TEvent< _Common > >
   {
      private:
         TEvent( const tID& id, const typename _Common::tData& data )
            : m_id( id )
            , m_data( data )
         {
         }
      public:
         ~TEvent( ) override = default;

      public:
         static std::shared_ptr< IEvent > create( const tID& id, const typename _Common::tData& data = { } )
         {
            return std::shared_ptr< IEvent >( new TEvent< _Common >( id, data ) );
         }

         static bool set_notification( std::shared_ptr< typename _Common::tConsumer > p_consumer, const tID& id )
         {
            return IEvent::set_notification( p_consumer, s_class_id, id );
         }

         static bool clear_notification( std::shared_ptr< typename _Common::tConsumer > p_consumer, const tID& id )
         {
            return IEvent::clear_notification( p_consumer, s_class_id, id );
         }

      public:
         bool send( ) override
         {
            return IEvent::send( TEvent< _Common >::shared_from_this( ) );
         }

         void process( std::shared_ptr< IEventConsumer > p_consumer ) override
         {
            std::static_pointer_cast< typename _Common::tConsumer >( p_consumer )->process( *this );
         }

      public:
         const tClassID& class_id( ) const override
         {
            return s_class_id;
         }
      private:
         static tClassID s_class_id;

      public:
         virtual const tID& id( ) const override
         {
            return m_id;
         }
      private:
         tID m_id;

      public:
         const typename _Common::tData& data( ) const
         {
            return m_data;
         }
      private:
         typename _Common::tData m_data;
   };



   template< typename _Data >
   struct TCommon
   {
      using tEvent      = TEvent< TCommon< _Data > >;
      using tData       = _Data;
      using tConsumer   = TEventConsumer< TCommon< _Data > >;
   };

} // namespace fw::event
