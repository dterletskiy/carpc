#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/comm/event/IEvent.hpp"
#include "api/sys/comm/service/Passport.hpp"
#include "api/sys/dsi/Types.hpp"



namespace base::application {

   class Context;



   class SendReceive
   {
      private:
         struct ProcessInfo
         {
            ProcessInfo( os::Socket::tSptr p_socket ) : socket( p_socket ) { }

            os::Socket::tSptr socket;
            bool confirmed = false;
            // service::Signature::tSet services;
         };
         using tProcessMapping = std::map< application::process::ID, ProcessInfo >;

         using tServiceMapping = std::map< os::Socket::tSptr, service::Passport::tList >;

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
         const os::Thread& thread( ) const;
         os::Thread                 m_thread;
         bool                       m_started = false;

      public:
         bool send( const RawBuffer&, const application::Context& );
         bool send( const dsi::Packet&, const application::Context& );
         bool send( const async::IEvent::tSptr, const application::Context& );
      private:
         bool send( const RawBuffer&, os::Socket::tSptr );
         bool send( const dsi::Packet&, os::Socket::tSptr );

      private:
         bool setup_connection( );
         os::Socket::tSptr socket( const application::Context& );
         os::Socket::tSptr          mp_socket_sb;
         os::Socket::tSptr          mp_socket_master;
         tServiceMapping            m_pending_sockets;
         tProcessMapping            m_process_mapping;

      private:
         void prepare_select( os::linux::socket::tSocket&, os::linux::socket::fd& );
         void process_select( os::linux::socket::tSocket&, os::linux::socket::fd& );
         bool process_stream( dsi::tByteStream&, os::Socket::tSptr );
         bool process_packet( dsi::Packet&, os::Socket::tSptr );
         bool process_package( dsi::Package&, os::Socket::tSptr );
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
