#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/comm/async/event/IEvent.hpp"
#include "api/sys/comm/service/Passport.hpp"
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
         const os::Thread& thread( ) const;
         os::Thread                 m_thread;
         bool                       m_started = false;

      public:
         bool send( const RawBuffer&, const application::Context& );
         bool send( const dsi::Packet&, const application::Context& );
         bool send( const async::IEvent::tSptr, const application::Context& );
      private:
         bool send( const RawBuffer&, os::Socket::tSptr );
         bool send( const ipc::tStream&, os::Socket::tSptr );
         bool send( const dsi::Packet&, os::Socket::tSptr );
         os::Socket::tSptr socket( const application::Context& );

      private:
         struct Base
         {
            Base( SendReceive& );
            virtual ~Base( ) = default;

            virtual bool setup_connection( ) = 0;

            virtual void prepare_select( os::os_linux::socket::tSocket&, os::os_linux::socket::fd& ) = 0;
            virtual void process_select( os::os_linux::socket::fd& ) = 0;

            bool process_stream( ipc::tStream& );
            bool process_packet( dsi::Packet& );
            virtual bool process_package( dsi::Package& ) = 0;

            SendReceive& m_parent;
         };

         // Structure for Connection to ServiceBrocker.
         // Contains client socket what should be connected to ServiceBrocket.
         // Via this socket current application will send information about provided and
         // required IPC interfaces to ServiceBrocker and will receive information about
         // other applications what provide required interfaces from ServiceBrocker.
         struct ServiceBrocker : public Base
         {
            ServiceBrocker( SendReceive& );

            bool setup_connection( ) override;

            void prepare_select( os::os_linux::socket::tSocket&, os::os_linux::socket::fd& ) override;
            void process_select( os::os_linux::socket::fd& ) override;

            bool process_package( dsi::Package& ) override;

            os::Socket::tSptr mp_socket = nullptr;
         };
         ServiceBrocker m_service_brocker;

         struct Master : public Base
         {
            Master( SendReceive& );

            bool setup_connection( ) override;

            void prepare_select( os::os_linux::socket::tSocket&, os::os_linux::socket::fd& ) override;
            void process_select( os::os_linux::socket::fd& ) override;

            bool process_package( dsi::Package& ) override;

            os::Socket::tSptr mp_socket = nullptr;
         };
         Master m_master;

         struct Connections : public Base
         {
            struct Channel
            {
               Channel( ) = default;
               Channel( os::Socket::tSptr p_socket_send, os::Socket::tSptr p_socket_recv )
                  : mp_socket_send( p_socket_send )
                  , mp_socket_recv( p_socket_recv )
               { }

               os::Socket::tSptr mp_socket_send;
               os::Socket::tSptr mp_socket_recv;

               bool established( ) const
               {
                  return ( nullptr != mp_socket_send ) && ( nullptr != mp_socket_recv );
               }

               service::Passport::tList pending_server_service_list;
               service::Passport::tList server_service_list;
               service::Passport::tList client_service_list;
            };
            using tProcessChannelMap = std::map< application::process::ID, Channel >;

            Connections( SendReceive& );

            bool setup_connection( ) override;

            void prepare_select( os::os_linux::socket::tSocket&, os::os_linux::socket::fd& ) override;
            void process_select( os::os_linux::socket::fd& ) override;

            bool process_package( dsi::Package& ) override;



            Channel& register_connection( const application::process::ID& pid );
            Channel& find_connection( const application::process::ID& pid );

            tProcessChannelMap               m_process_channel_map;
            tProcessChannelMap::iterator     m_process_channel_map_iterator;



            void add_pending_socket( os::Socket::tSptr& );

            os::Socket::tSptrList            m_pending_sockets;
            os::Socket::tSptrList::iterator  m_pending_sockets_iterator;
         };
         Connections m_connections;
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
