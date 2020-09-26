#pragma once

#include "api/sys/oswrappers/Types.hpp"
#include "api/sys/oswrappers/linux/socket.hpp"
#include "api/sys/oswrappers/linux/timer.hpp"
#include "api/sys/service/ServiceThread.hpp"
#include "api/sys/comm/interface/ConnectionDB.hpp"



namespace base {

   class ServiceProcess
      : public std::enable_shared_from_this< ServiceProcess >
   {
      public:
         using tSptr = std::shared_ptr< ServiceProcess >;
         using tWptr = std::weak_ptr< ServiceProcess >;
         using tUptr = std::unique_ptr< ServiceProcess >;
         using tRptr = ServiceProcess*;

      public:
         ~ServiceProcess( );
         static tSptr instance( );
      private:
         ServiceProcess( );
         ServiceProcess( const ServiceProcess& ) = delete;
         ServiceProcess& operator=( const ServiceProcess& ) = delete;
         static tSptr mp_instance;

      public:
         bool start( const ServiceThread::Info::tVector& );
         bool stop( );
         void boot( );

      public:
         const ID& id( ) const;
         const std::string& name( ) const;
      private:
         ID             m_id = InvalidID;
         std::string    m_name = "NoName";

      public:
         IServiceThread::tSptr service_ipc( ) const;
      private:
         IServiceThread::tSptr   mp_service_ipc = nullptr;

      public:
         IServiceThread::tSptr service( const ID& ) const;
         IServiceThread::tSptr current_service( ) const;
         IServiceThread::tSptrList service_list( ) const;
      private:
         IServiceThread::tSptrList  m_service_list;

      public:
         interface::ConnectionDB& connection_db( );
      private:
         interface::ConnectionDB    m_connection_db;

      private:
         os::linux::timer::tID      m_timer_id;

      public:
         struct Configuration
         {
            os::linux::socket::configuration ipc_sb;
            size_t ipc_sb_buffer_size;
            os::linux::socket::configuration ipc_app;
            size_t ipc_app_buffer_size;
         };
      public:
         const Configuration& configuration( ) const;
      private:
         Configuration m_configuration;
   };



   inline
   const std::string& ServiceProcess::name( ) const
   {
      return m_name;
   }

   inline
   const ID& ServiceProcess::id( ) const
   {
      return m_id;
   }

   inline
   IServiceThread::tSptrList ServiceProcess::service_list( ) const
   {
      return m_service_list;
   }

   inline
   interface::ConnectionDB& ServiceProcess::connection_db( )
   {
      return m_connection_db;
   }

   inline
   const ServiceProcess::Configuration& ServiceProcess::configuration( ) const
   {
      return m_configuration;
   }


} // namespace base
