#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>

#include "api/sys/configuration/DSI.hpp"
#include "api/sys/comm/Event.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceBrocker.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceBrockerDSI.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvBrDsi"



namespace base {



ServiceBrockerDsiPtr ServiceBrockerDSI::mp_instance;

ServiceBrockerDSI::ServiceBrockerDSI( )
   : m_events( )
{
   mp_thread_send = std::make_shared< base::os::Thread >( std::bind( &ServiceBrockerDSI::thread_loop_send, this ) );
   mp_thread_receive = std::make_shared< base::os::Thread >( std::bind( &ServiceBrockerDSI::thread_loop_receive, this ) );

   SYS_INF( "Created" );
}

ServiceBrockerDSI::~ServiceBrockerDSI( )
{
   SYS_INF( "Destroyed" );
}

ServiceBrockerDsiPtr ServiceBrockerDSI::instance( )
{
   os::Mutex mutex( true );
   if( !mp_instance )
   {
      mp_instance.reset( new ServiceBrockerDSI( ) );
   }

   return mp_instance;
}

bool ServiceBrockerDSI::setup_connection( )
{
   m_master_socket = socket( base::configuration::dsi::socket_family, base::configuration::dsi::socket_type, base::configuration::dsi::socket_protocole );
   if( -1 == m_master_socket )
   {
      m_last_errno = errno;
      SYS_ERR( "socket(%d) error: %d", m_master_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "socket(%d)", m_master_socket );

   sockaddr* p_sockaddr = nullptr;
   size_t sockaddr_size = 0;
   struct sockaddr_un serv_addr_un;
   struct sockaddr_in serv_addr_in;
   if( AF_UNIX == base::configuration::dsi::socket_family )
   {
      memset( &serv_addr_un, 0, sizeof( serv_addr_un ) );
      serv_addr_un.sun_family = base::configuration::dsi::socket_family;
      strncpy( serv_addr_un.sun_path, base::configuration::dsi::server_address, sizeof( serv_addr_un.sun_path ) - 1 );
      // unlink( base::configuration::dsi::server_address );
      sockaddr_size = sizeof( serv_addr_un.sun_family ) + strlen( serv_addr_un.sun_path );

      p_sockaddr = reinterpret_cast< sockaddr* >( &serv_addr_un );
   }
   else if( AF_INET == base::configuration::dsi::socket_family )
   {
      memset( &serv_addr_in, 0, sizeof( serv_addr_in ) );
      serv_addr_in.sin_family = base::configuration::dsi::socket_family;
      serv_addr_in.sin_addr.s_addr = inet_addr( base::configuration::dsi::server_address );
      // serv_addr_in.sin_addr.s_addr = htonl( INADDR_ANY );
      // serv_addr_in.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
      serv_addr_in.sin_port = htons( base::configuration::dsi::server_port );
      sockaddr_size = sizeof( serv_addr_in );

      p_sockaddr = reinterpret_cast< sockaddr* >( &serv_addr_in );
   }

   int result_connect = connect( m_master_socket, p_sockaddr, sockaddr_size );
   if( -1 == result_connect )
   {
      m_last_errno = errno;
      SYS_ERR( "connect(%d): %d", m_master_socket, m_last_errno );
      return false;
   }
   SYS_MSG( "connect(%d)", m_master_socket );

   return true;
}

bool ServiceBrockerDSI::start( )
{
   if( false == setup_connection( ) )
      return false;

   SYS_INF( "Starting ServiceBrockerDSI" );
   bool result_send = start_send( );
   bool result_receive = start_receive( );

   return result_send && result_receive;
}

void ServiceBrockerDSI::stop( )
{
   SYS_INF( "Stopping ServiceBrockerDSI" );
   stop_send( );
   stop_receive( );
}

void ServiceBrockerDSI::thread_loop_send( )
{
   SYS_INF( "enter" );
   m_started_send = true;

   // ByteBufferT register_buffer;
   // for( const auto& pair : EventRegistry::instance( )->registry( ) )
   // {
   //    if( false == register_buffer.push( pair.first ) )
   //    {
   //       SYS_ERR( "registration error" );
   //       return;
   //    }
   // }
   // size_t send_size = send( m_master_socket, register_buffer.buffer( ), register_buffer.size( ), 0 );
   // m_last_errno = errno;
   // if( send_size != register_buffer.size( ) )
   // {
   //    SYS_ERR( "send(%d): %d", m_master_socket, m_last_errno );
   //    SYS_ERR( "registration error" );
   //    return;
   // }
   // SYS_MSG( "send(%d): %zu bytes", m_master_socket, send_size );

   while( started_send( ) )
   {
      EventPtr p_event = get_event( );
      SYS_INF( "processing event (%s)", p_event->type_id( ).c_str( ) );
      base::ByteBufferT buffer;
      if( false == p_event->to_buffer( buffer ) )
      {
         SYS_ERR( "lost sent event" );
         continue;
      }
      // buffer.dump( );

      size_t send_size = send( m_master_socket, buffer.buffer( ), buffer.size( ), 0 );
      m_last_errno = errno;
      if( send_size != buffer.size( ) )
      {
         SYS_ERR( "send(%d): %d", m_master_socket, m_last_errno );
         continue;
      }
      SYS_MSG( "send(%d): %zu bytes", m_master_socket, send_size );
   }

   SYS_INF( "exit" );
}

bool ServiceBrockerDSI::start_send( )
{
   SYS_INF( "Starting ServiceBrockerDSI send thread" );
   bool result = mp_thread_send->run( );
   if( false == result )
   {
      SYS_ERR( "ServiceBrockerDSI can't start send thread" );
   }

   return result;
}

void ServiceBrockerDSI::stop_send( )
{
   SYS_INF( "Stopping ServiceBrockerDSI send thread" );
   m_started_send = false;
}

void ServiceBrockerDSI::thread_loop_receive( )
{
   SYS_INF( "enter" );

   ServiceBrockerPtr p_service_brocker = ServiceProcess::instance( )->service_brocker( );
   if( InvalidServiceBrockerPtr == p_service_brocker )
      return;

   void* p_buffer = malloc( base::configuration::dsi::buffer_size );
   if( nullptr == p_buffer )
      return;

   m_started_receive = true;

   while( started_receive( ) )
   {
      memset( p_buffer, 0, sizeof( base::configuration::dsi::buffer_size ) );
      ssize_t recv_size = recv( m_master_socket, p_buffer, base::configuration::dsi::buffer_size, 0 );
      if( 0 >= recv_size )
      {
         m_last_errno = errno;
         SYS_ERR( "recv(%d): %d", m_master_socket, m_last_errno );
         continue;
      }
      SYS_MSG( "recv(%d): %ld bytes", m_master_socket, recv_size );

      ByteBufferT byte_buffer( p_buffer, recv_size );
      // byte_buffer.dump( );

      base::EventPtr p_event = base::EventRegistry::instance( )->create_event( byte_buffer );
      if( nullptr == p_event )
      {
         SYS_ERR( "lost received event" );
         continue;
      }
      p_service_brocker->insert_event( p_event );
   }

   free( p_buffer );
   SYS_INF( "exit" );
}

bool ServiceBrockerDSI::start_receive( )
{
   SYS_INF( "Starting ServiceBrockerDSI receive thread" );
   bool result = mp_thread_receive->run( );
   if( false == result )
   {
      SYS_ERR( "ServiceBrockerDSI can't start receive thread" );
   }

   return result;
}

void ServiceBrockerDSI::stop_receive( )
{
   SYS_INF( "Stopping ServiceBrockerDSI receive thread" );
   m_started_receive = false;
}

bool ServiceBrockerDSI::insert_event( const EventPtr p_event )
{
   if( false == m_started_send )
   {
      SYS_WRN( "ServiceBrockerDSI send thread is not started" );
      return false;
   }

   m_buffer_cond_var.lock( );
   m_events.push_back( p_event );
   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );
   return true;
}

EventPtr ServiceBrockerDSI::get_event( )
{
   m_buffer_cond_var.lock( );
   if( true == m_events.empty( ) )
   {
      m_buffer_cond_var.wait( );
   }
   EventPtr p_event = m_events.front( );
   m_events.pop_front( );
   m_buffer_cond_var.unlock( );

   return p_event;
}

void ServiceBrockerDSI::notify( const EventPtr p_event )
{
}



} // namespace base
