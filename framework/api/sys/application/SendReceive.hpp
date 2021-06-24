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
         bool send( const dsi::Packet&, os::Socket::tSptr );
         os::Socket::tSptr socket( const application::Context& );

      private:
         struct Channel
         {
            os::Socket::tSptr socket_send;
            os::Socket::tSptr socket_recv;

            service::Passport::tList servers;
            service::Passport::tList clients;
         };
         using tProcessChannelMap = std::map< application::process::ID, Channel >;

         os::Socket::tSptrList   m_incoming_sockets;
         tProcessChannelMap      m_process_channel_map;

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

         struct PendingConnections : public Base
         {
            PendingConnections( SendReceive& );

            bool setup_connection( ) override;

            void prepare_select( os::os_linux::socket::tSocket&, os::os_linux::socket::fd& ) override;
            void process_select( os::os_linux::socket::fd& ) override;

            bool process_package( dsi::Package& ) override;

            void add( os::Socket::tSptr& );

            os::Socket::tSptrList            m_sockets;
            os::Socket::tSptrList::iterator  m_sockets_iterator;
         };
         PendingConnections m_pending_connections;

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

            Channel& register_connection( const application::process::ID& pid )
            {
               auto [ iterator, success ] = m_process_channel_map.insert( { pid, Channel( ) } );
               return iterator->second;
            }

            Channel& find_connection( const application::process::ID& pid )
            {
               static Channel invalid_channel;
               auto iterator = m_process_channel_map.find( pid );
               return m_process_channel_map.end( ) == iterator ? invalid_channel : iterator->second;
            }

            tProcessChannelMap            m_process_channel_map;
            tProcessChannelMap::iterator  m_process_channel_map_iterator;
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
