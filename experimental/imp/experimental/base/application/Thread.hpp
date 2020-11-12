#pragma once

#include <vector>
#include <memory>
#include <thread>
#include "imp/experimental/base/application/Context.hpp"
#include "imp/experimental/base/component/Base.hpp"



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
   };

} // namespace fw::application
