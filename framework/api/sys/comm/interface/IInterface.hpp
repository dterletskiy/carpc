#pragma once

#include <string>

#include "api/sys/service/IServiceThread.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/comm/event/Runnable.hpp"
#include "api/sys/comm/interface/Types.hpp"
#include "api/sys/events/Events.hpp"



namespace base {

   class IInterface : public events::interface::Interface::Consumer
   {
      public:
         IInterface( const std::string&, const std::string&, const eCommType );
         virtual ~IInterface( );

      public:
         virtual void connected( const void* const ) = 0;
         virtual void disconnected( const void* const ) = 0;
         virtual void connected( ) = 0;
         virtual void disconnected( ) = 0;
         virtual const bool is_connected( ) const = 0;

      protected:
         bool register_server( ) const;
         bool register_client( ) const;

      private:
         void process_event( const events::interface::Interface::Event& ) override;

      protected:
         eCommType   m_comm_type = eCommType::NONE;

      public:
         const std::string& name( ) const;
         const std::string& role( ) const;
         const std::string service_name( ) const;
         const bool cmp_names( const IInterface* const ) const;
      private:
         std::string m_name = "";
         std::string m_role = "";

      public:
         IServiceThread::tWptr context( ) const;
      private:
         IServiceThread::tWptr mp_service_thread;
   };



   inline
   const std::string& IInterface::name( ) const
   {
      return m_name;
   }

   inline
   const std::string& IInterface::role( ) const
   {
      return m_role;
   }

   inline
   const std::string IInterface::service_name( ) const
   {
      return m_role + std::string{"."} + m_name;
   }

   inline
   const bool IInterface::cmp_names( const IInterface* const p_interface ) const
   {
      return ( p_interface->name( ) == m_name ) && ( p_interface->role( ) == m_role );
   }

   inline
   IServiceThread::tWptr IInterface::context( ) const
   {
      return mp_service_thread;
   }

} // namespace base
