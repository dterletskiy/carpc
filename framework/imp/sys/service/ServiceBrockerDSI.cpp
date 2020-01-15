#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>

#include "api/sys/comm/Event.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceBrocker.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceBrockerDSI.hpp"
#include "api/sys/service/Configuration.hpp"

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
   m_master_socket = socket( socket_family, socket_type, socket_protocole );

   int result_connect = -1;
   if( AF_UNIX == socket_family )
   {
      struct sockaddr_un serv_addr_un;
      memset( &serv_addr_un, 0, sizeof(serv_addr_un) );
      serv_addr_un.sun_family = socket_family;
      strncpy( serv_addr_un.sun_path, server_address, sizeof(serv_addr_un.sun_path) - 1 );
      int serv_addr_un_size = sizeof(serv_addr_un.sun_family) + strlen(serv_addr_un.sun_path);

      result_connect = connect( m_master_socket, (struct sockaddr*) &serv_addr_un, serv_addr_un_size );
   }
   else if( AF_INET == socket_family )
   {
      struct sockaddr_in serv_addr_in;
      memset( &serv_addr_in, 0, sizeof(serv_addr_in) );
      serv_addr_in.sin_family = socket_family;
      // serv_addr_in.sin_addr.s_addr = inet_addr(inet_address);
      serv_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr_in.sin_port = htons(server_port);
      int serv_addr_in_size = sizeof(serv_addr_in);

      result_connect = connect( m_master_socket, (struct sockaddr*) &serv_addr_in, serv_addr_in_size );
   }

   if( 0 != result_connect )
   {
      SYS_ERR( "connect(%d): %d", m_master_socket, errno );
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
   int last_error = 0;

   while( started_send( ) )
   {
      EventPtr p_event = get_event( );
      SYS_INF( "processing event (%s)", p_event->type_id( ).c_str( ) );
      base::ByteBuffer buffer;
      p_event->to_buffer( buffer );
      buffer.dump( );

      size_t send_size = send( m_master_socket, buffer.buffer( ), buffer.size( ), 0 );
      last_error = errno;
      if( send_size != buffer.size( ) )
      {
         SYS_ERR( "send(%d): %d", m_master_socket, last_error );
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

   const size_t buffer_size = 2048;
   void* p_buffer = malloc( buffer_size );
   if( nullptr == p_buffer )
      return;

   m_started_receive = true;
   int last_error = 0;

   while( started_receive( ) )
   {
      memset( p_buffer, 0, sizeof( buffer_size ) );
      ssize_t recv_size = recv( m_master_socket, p_buffer, buffer_size, 0 );
      last_error = errno;
      if( 0 >= recv_size )
      {
         SYS_ERR( "recv(%d): %d", m_master_socket, last_error );
         continue;
      }
      SYS_MSG( "recv(%d): %ld bytes", m_master_socket, recv_size );

      ByteBuffer byte_buffer( p_buffer, recv_size );
      byte_buffer.dump( );

      base::EventPtr p_event = base::EventRegistry::instance( )->create_event( byte_buffer );
      p_service_brocker->insert_event( p_event );
   }

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
