#pragma once

#include <set>

#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/common/Includes.hpp"
#include "api/sys/comm/service/Address.hpp"
#include "api/sys/comm/service/Signature.hpp"
#include "api/sys/comm/service/Passport.hpp"
#include "api/sys/common/Packet.hpp"



struct Connection
{
   using tSet = std::set< Connection >;
   using tOpt = std::optional< Connection >;

   bool operator<( const Connection& other ) const { return socket < other.socket; }

   carpc::os::Socket::tSptr socket = nullptr;
   carpc::service::Address  service_address;
   carpc::ipc::SocketCongiguration inet_address;
};

struct ServiceConnection
{
   Connection::tOpt server = std::nullopt;
   Connection::tSet clients;
};

class ConnectionProcessor : public carpc::os::SocketServer::IConsumer
{
   public:
      ConnectionProcessor( const carpc::os::os_linux::socket::configuration& configuration, const size_t buffer_size );
      ~ConnectionProcessor( );

      bool init( );
      bool shutdown( );
      void connection_loop( );

   private:
      void read_slave( carpc::os::Socket::tSptr ) override;
      void connected( carpc::os::Socket::tSptr ) override;
      void disconnected( carpc::os::Socket::tSptr ) override;

   private:
      void process_broadcast_event( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package );
      void register_server( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package );
      void unregister_server( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package );
      void register_client( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package );
      void unregister_client( carpc::os::Socket::tSptr p_socket, carpc::ipc::Package& package );

   private:
      carpc::os::SocketServer::tSptr                            mp_socket;
      std::map< carpc::service::Signature, ServiceConnection >  m_service_map;
};
