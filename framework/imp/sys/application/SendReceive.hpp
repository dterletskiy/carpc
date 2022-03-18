#pragma once

#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/comm/async/event/IEvent.hpp"
#include "api/sys/comm/service/Passport.hpp"
#include "api/sys/dsi/Types.hpp"



namespace carpc::application {

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
         std::atomic< bool >        m_started = false;

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

            bool process_stream( ipc::tStream&, os::Socket::tSptr );
            bool process_packet( dsi::Packet&, os::Socket::tSptr );
            virtual bool process_package( dsi::Package&, os::Socket::tSptr ) = 0;

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

            bool process_package( dsi::Package&, os::Socket::tSptr ) override;

            os::Socket::tSptr mp_socket = nullptr;
         };
         ServiceBrocker m_service_brocker;

         struct Master : public Base
         {
            Master( SendReceive& );

            bool setup_connection( ) override;

            void prepare_select( os::os_linux::socket::tSocket&, os::os_linux::socket::fd& ) override;
            void process_select( os::os_linux::socket::fd& ) override;

            bool process_package( dsi::Package&, os::Socket::tSptr ) override;

            os::Socket::tSptr mp_socket = nullptr;
         };
         Master m_master;

         struct Connections : public Base
         {
            #if 0 // perhaps redesign
               struct Connection
               {
                  using tProcessConnectionMap = std::map< application::process::ID, Connection >;

                  using tSocketConnectionMap = std::map< os::Socket::tSptr, Connection >;
                  using tServiceConnectionMap = std::map< service::Passport::tSet, Connection >;

                  tSocketConnectionMap::iterator send_iterator = ms_send.end( );
                  tSocketConnectionMap::iterator recv_iterator = ms_recv.end( );
                  tServiceConnectionMap::iterator pending_servers_iterator = ms_pending_servers.end( );
                  tServiceConnectionMap::iterator servers_iterator = ms_servers.end( );
                  tServiceConnectionMap::iterator clients_iterator = ms_clients.end( );
               };

               Connection::tProcessConnectionMap ms_process;

               Connection::tSocketConnectionMap ms_send;
               Connection::tSocketConnectionMap ms_recv;

               Connection::tServiceConnectionMap ms_pending_servers;
               Connection::tServiceConnectionMap ms_servers;
               Connection::tServiceConnectionMap ms_clients;
            #endif

            private:
               using tSocketProcessMap = std::map< os::Socket::tSptr, application::process::ID >;
               using tProcessSocketMap = std::map< application::process::ID, os::Socket::tSptr >;
               using tProcessServiceMap = std::map< application::process::ID, service::Passport::tSet >;

            private:
               struct data
               {
                  static tProcessSocketMap ms_send;
                  static tSocketProcessMap ms_recv;

                  static tProcessServiceMap ms_pending_servers;
                  static tProcessServiceMap ms_servers;
                  static tProcessServiceMap ms_clients;
               };

            public:
               struct channel // => socket
               {
                  struct send
                  {
                     static os::Socket::tSptr create( const application::process::ID& pid, const dsi::SocketCongiguration& inet_address );
                     static bool remove( const application::process::ID& pid );
                     static os::Socket::tSptr socket( const application::process::ID& pid );
                     static const application::process::ID& pid( os::Socket::tSptr p_socket );
                     static tProcessSocketMap& collection( );
                  };

                  struct recv
                  {
                     static bool add( os::Socket::tSptr p_socket );
                     static bool update( os::Socket::tSptr p_socket, const application::process::ID& pid );
                     static bool remove( os::Socket::tSptr p_socket );
                     static const application::process::ID& pid( os::Socket::tSptr p_socket );
                     static os::Socket::tSptr socket( const application::process::ID& pid );
                     static tSocketProcessMap& collection( );
                  };

                  static bool established( const application::process::ID& pid );
               };

               struct interface // => service
               {
                  enum class eType: std::uint8_t { SERVER, CLIENT, PENDING_SERVER, PENDING_CLIENT };

                  template< eType type >
                  struct base
                  {
                     static tProcessServiceMap& collection( )
                     {
                        switch( type )
                        {
                           case eType::SERVER:           return data::ms_servers;
                           case eType::CLIENT:           return data::ms_clients;
                           case eType::PENDING_SERVER:   return data::ms_pending_servers;
                        }
                        static tProcessServiceMap dummy;
                        return dummy;
                     }
                     static const service::Passport::tSet& passports( const application::process::ID& pid )
                     {
                        auto& process_service_map = collection( );

                        auto iterator = process_service_map.find( pid );
                        if( process_service_map.end( ) == iterator )
                        {
                           static const service::Passport::tSet dummy{ };
                           return dummy;
                        }

                        return iterator->second;                           
                     }
                     static bool add( const application::process::ID& pid, const service::Passport& passport )
                     {
                        auto& process_service_map = collection( );

                        auto iterator = process_service_map.find( pid );
                        if( process_service_map.end( ) == iterator )
                           return process_service_map.emplace( pid, service::Passport::tSet{ passport } ).second;

                        return iterator->second.emplace( passport ).second;
                     }
                     static bool remove( const application::process::ID& pid, const service::Passport& passport )
                     {
                        auto& process_service_map = collection( );

                        auto iterator = process_service_map.find( pid );
                        if( process_service_map.end( ) == iterator )
                           return false;

                        return 0 != iterator->second.erase( passport );
                     }
                     static bool remove( const application::process::ID& pid )
                     {
                        auto& process_service_map = collection( );

                        auto iterator = process_service_map.find( pid );
                        if( process_service_map.end( ) == iterator )
                           return false;

                        iterator->second.clear( );
                        return true;
                     }
                     static const service::Passport::tSet& find( const application::process::ID& pid )
                     {
                        auto& process_service_map = collection( );

                        auto iterator = process_service_map.find( pid );
                        if( process_service_map.end( ) == iterator )
                        {
                           static const service::Passport::tSet dummy;
                           return dummy;
                        }

                        return iterator->second;
                     }
                     static bool process( const application::process::ID& pid )
                     {
                        auto& process_service_map = collection( );

                        return true;
                     }
                  };
                  struct server : public base< eType::SERVER >
                  {
                     struct pending : public base< eType::PENDING_SERVER > { };
                  };
                  struct client : public base< eType::CLIENT >
                  {
                     struct pending : public base< eType::PENDING_CLIENT > { };
                  };
               };

            public:
               Connections( SendReceive& );

               bool setup_connection( ) override;

               void prepare_select( os::os_linux::socket::tSocket&, os::os_linux::socket::fd& ) override;
               void process_select( os::os_linux::socket::fd& ) override;

               bool process_package( dsi::Package&, os::Socket::tSptr ) override;
         };
         Connections m_connections;
   };



   inline
   bool SendReceive::started( ) const
   {
      return m_started.load( );
   }

   inline
   bool SendReceive::wait( )
   {
      const bool started = m_thread.join( );
      m_started.store( started );
      return !m_started.load( );
   }

} // namespace carpc::application
