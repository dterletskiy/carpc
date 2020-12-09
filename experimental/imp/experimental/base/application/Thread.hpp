#pragma once

#include <vector>
#include <list>
#include <set>
#include <memory>
#include <thread>
#include "imp/experimental/base/application/Context.hpp"
#include "imp/experimental/base/component/Base.hpp"
#include "imp/experimental/base/event/IEvent.hpp"



namespace fw::application {

   class Thread
   {
      public:
         struct Configuration
         {
            std::string       name;
            std::vector< component::Base::tCreator >
                              component_creators;
         };

      public:
         Thread( const std::vector< component::Base::tCreator >&, const std::string& name = "NoName" );
         ~Thread( );

         void run( );
         void wait( );
         void stop( );
         void post( Context::tFunction function );

      private:
         void execute( );

      public:
         const std::string& name( ) const;
      private:
         std::string       m_name;

      public:
         std::thread::id id( ) const;
      private:
         std::thread       m_thread;
         Context           m_context;

      private:
         std::vector< std::unique_ptr< component::Base > >  m_components;
         std::vector< component::Base::tCreator >           m_component_creators;

      public:
         bool set_notification( const event::Info&, std::shared_ptr< event::IEventConsumer > );
         bool clear_notification( const event::Info&, std::shared_ptr< event::IEventConsumer > );
      private:
         struct Comparator
         {
            bool operator( )( const std::weak_ptr< event::IEventConsumer > wp_1, const std::weak_ptr< event::IEventConsumer > wp_2 ) const
            {
               auto sp_1 = wp_1.lock( );
               auto sp_2 = wp_2.lock( );
               if( !sp_1 || !sp_2 )
                  return false;
               return sp_1 < sp_2;
            }
         };
      private:
         using tEventConsumers = std::set< std::weak_ptr< event::IEventConsumer >, Comparator >;
         std::map< event::Info, tEventConsumers >   m_subscriptions;

      public:
         bool insert_event( std::shared_ptr< event::IEvent > );
      private:
         std::list< std::shared_ptr< event::IEvent > > m_events;
   };

} // namespace fw::application
