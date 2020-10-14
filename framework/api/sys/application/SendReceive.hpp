#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/dsi/Types.hpp"



namespace base::application {

   class Context;



   class SendReceive
   {
      public:
         SendReceive( );
         ~SendReceive( );
      private:
         SendReceive( const SendReceive& ) = delete;
         SendReceive& operator=( const SendReceive& ) = delete;

      public:
         bool start( );
         void stop( );
         bool started( ) const;
         bool wait( );
      private:
         void thread_loop( );
         const base::os::Thread& thread( ) const;
         os::Thread                 m_thread;
         bool                       m_started = false;

      public:
         bool send( const application::Context&, const base::RawBuffer& );

      private:
         bool setup_connection( );
         os::Socket                 m_socket_sb;
         os::Socket                 m_socket_master;
         os::Socket::tSptrList      m_sockets_slave;
         os::Socket::tSptrList      m_sockets_client;

      private:
         void prepare_select( os::linux::socket::tSocket&, os::linux::socket::fd& );
         void process_select( os::linux::socket::tSocket&, os::linux::socket::fd& );
         bool process_stream( dsi::tByteStream&, os::Socket& );
         bool process_packet( dsi::Packet&, os::Socket& );
         bool process_package( dsi::Package&, os::Socket& );
   };



   inline
   bool SendReceive::started( ) const
   {
      return m_started;
   }

   inline
   bool SendReceive::wait( )
   {
      m_started = m_thread.join( );
      return !m_started;
   }

} // namespace base::application
