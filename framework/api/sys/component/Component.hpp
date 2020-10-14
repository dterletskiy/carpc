#pragma once

#include "api/sys/component/IComponent.hpp"
#include "api/sys/tools/Performance.hpp"



namespace base::component {

   class Component : public IComponent
   {
      public:
         Component( application::IThread&, const std::string& );
         ~Component( ) override;

      private:
         const bool is_root( ) const override { return false; }
         void boot( const std::string& info = "boot" ) override { }
         void shutdown( const std::string& info = "shutdown" ) override { }

      public:
         const std::string& name( ) const override final;
      private:
         std::string                m_name;

      public:
         const ID& id( ) const override final;
      private:
         ID                         m_id;

      private:
         application::IThread&      m_app_thread;

      protected:
         void start_performance( const std::string& message = "" );
         void stop_performance( const std::string& message = "" );
      private:
         base::tools::Performance   m_performance;
   };



   inline
   const std::string& Component::name( ) const
   {
      return m_name;
   }

   inline
   const ID& Component::id( ) const
   {
      return m_id;
   }

   inline
   void Component::start_performance( const std::string& message )
   {
      m_performance.start( message );
   }

   inline
   void Component::stop_performance( const std::string& message )
   {
      m_performance.stop( message );
   }

} // namespace base::component
