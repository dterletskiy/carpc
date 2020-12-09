#pragma once

#include <list>
#include <memory>

#include "imp/experimental/base/application/Thread.hpp"



namespace fw::application {

   class Application
   {
      public:
         ~Application( );
      private:
         Application( );
         Application( const Application& ) = delete;
         Application& operator=( const Application& ) = delete;

      public:
         static Application& instance( );
      private:
         static std::unique_ptr< Application > mp_instance;

      public:
         void run( const std::list< Thread::Configuration >& );

      public:
         std::shared_ptr< Thread > current_thread( ) const;
         const std::list< std::shared_ptr< Thread > >& threads( ) const;
      private:
         std::list< std::shared_ptr< Thread > > m_threads;
   };

} // namespace fw::application
