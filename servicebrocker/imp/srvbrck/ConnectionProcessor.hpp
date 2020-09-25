#pragma once

#include <set>

#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/common/Types.hpp"
#include "api/sys/dsi/Types.hpp"
#include "api/sys/comm/interface/Types.hpp"



struct Connection
{
   using tSet = std::set< Connection >;
   using tOpt = std::optional< Connection >;

   bool operator<( const Connection& other ) const { return socket < other.socket; }

   base::os::Socket::tSptr socket = nullptr;
   const void*             ptr = nullptr;
   std::string             address;
   int                     port;
   // base::os::linux::socket::configuration;
};

struct ServiceConnection
{
   Connection::tOpt server = std::nullopt;
   Connection::tSet clients;
};

class ConnectionProcessor : public base::os::SocketServer
{
   private:
      enum class eRead : size_t { OK, ERROR, DISCONNECTED };

   public:
      ConnectionProcessor( const base::os::linux::socket::configuration& configuration, const size_t buffer_size );
      ~ConnectionProcessor( );

      bool init( );
      bool shutdown( );
      void connection_loop( );
      int execute( );

   private:
      void read_slave( base::os::Socket::tSptr ) override;

   private:
      void process_broadcast_event( base::os::Socket::tSptr p_socket, base::dsi::Package& package );
      void register_server( base::os::Socket::tSptr p_socket, base::dsi::Package& package );
      void unregister_server( base::os::Socket::tSptr p_socket, base::dsi::Package& package );
      void register_client( base::os::Socket::tSptr p_socket, base::dsi::Package& package );
      void unregister_client( base::os::Socket::tSptr p_socket, base::dsi::Package& package );

   private:
      std::map< base::interface::Signature, ServiceConnection > m_service_map;
};
