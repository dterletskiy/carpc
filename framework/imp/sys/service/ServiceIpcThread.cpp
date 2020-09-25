#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/dsi/Types.hpp"
#include "api/sys/comm/interface/Types.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceIpcThread.hpp"
#include "imp/sys/service/ServiceEventConsumer.hpp"
#include "imp/sys/service/InterfaceEventConsumer.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvIPC"



namespace ev_i = base::events::interface;
using namespace base;



ServiceIpcThread::ServiceIpcThread( )
   : m_name( "IPC" )
   , m_wd_timeout( 10 )
   , m_events( )
   , m_buffer_cond_var( )
   , m_event_consumers_map( )
   , m_socket_sb( ServiceProcess::instance( )->configuration( ).ipc_sb, ServiceProcess::instance( )->configuration( ).ipc_sb_buffer_size )
   , m_socket_master( ServiceProcess::instance( )->configuration( ).ipc_app, ServiceProcess::instance( )->configuration( ).ipc_app_buffer_size )
{
   mp_thread_send = std::make_shared< base::os::Thread >( std::bind( &ServiceIpcThread::thread_loop_send, this ) );
   mp_thread_receive = std::make_shared< base::os::Thread >( std::bind( &ServiceIpcThread::thread_loop_receive, this ) );

   SYS_TRC( "'%s': created", m_name.c_str( ) );
}

ServiceIpcThread::~ServiceIpcThread( )
{
   SYS_TRC( "'%s': destroyed", m_name.c_str( ) );
}

void ServiceIpcThread::thread_loop_send( )
{
   SYS_INF( "'%s': enter", m_name.c_str( ) );
   m_started_send = true;

   mp_service_event_consumer = std::make_shared< ServiceEventConsumer >( *this );
   mp_interface_event_consumer = std::make_shared< InterfaceEventConsumer >( *this );

   while( started_send( ) )
   {
      base::async::IAsync::tSptr p_event = get_event( );
      SYS_TRC( "'%s': processing event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
      notify( p_event );
   }

   mp_service_event_consumer.reset( );
   mp_interface_event_consumer.reset( );

   SYS_INF( "'%s': exit", m_name.c_str( ) );
}

void ServiceIpcThread::thread_loop_receive( )
{
   SYS_INF( "'%s': enter", m_name.c_str( ) );
   m_started_receive = true;

   os::linux::socket::fd fd_set;
   os::linux::socket::tSocket max_socket = os::linux::socket::InvalidSocket;

   while( started_receive( ) )
   {
      fd_set.reset( );
      fd_set.set( m_socket_sb.socket( ), os::linux::socket::fd::eType::READ );
      max_socket = m_socket_sb.socket( );
      fd_set.set( m_socket_master.socket( ), os::linux::socket::fd::eType::READ );
      if( m_socket_master.socket( ) > max_socket )
         max_socket = m_socket_master.socket( );
      for( const auto& p_slave_socket : m_sockets_slave )
      {
         fd_set.set( p_slave_socket->socket( ), os::linux::socket::fd::eType::READ );
         if( p_slave_socket->socket( ) > max_socket )
            max_socket = p_slave_socket->socket( );
      }
      for( const auto& p_client_socket : m_sockets_client )
      {
         fd_set.set( p_client_socket->socket( ), os::linux::socket::fd::eType::READ );
         if( p_client_socket->socket( ) > max_socket )
            max_socket = p_client_socket->socket( );
      }
      SYS_INF( "max_socket = %d", max_socket );

      if( false == os::linux::socket::select( max_socket, fd_set ) )
         continue;

      // process service brocker sockets
      if( true == fd_set.is_set( m_socket_sb.socket( ), os::linux::socket::fd::eType::READ ) )
      {
         const os::Socket::eResult result = m_socket_sb.recv( );
         if( os::Socket::eResult::DISCONNECTED == result )
         {
            m_socket_sb.info( "Host disconnected" );
         }
         else if( os::Socket::eResult::OK == result )
         {
            size_t recv_size = 0;
            const void* const p_buffer = m_socket_sb.buffer( recv_size );
            dsi::tByteStream stream;
            stream.push( p_buffer, recv_size );

            while( 0 < stream.size( ) )
            {
               dsi::Packet packet;
               stream.pop( packet );
               process_packet( packet, m_socket_sb );
            }
         }
      }

      // process slave sockets
      for( auto iterator = m_sockets_slave.begin( ); iterator != m_sockets_slave.end( ); ++iterator )
      {
         if( false == fd_set.is_set( (*iterator)->socket( ), os::linux::socket::fd::eType::READ ) )
            continue;

         const os::Socket::eResult result = (*iterator)->recv( );
         if( os::Socket::eResult::DISCONNECTED == result )
         {
            (*iterator)->info( "Host disconnected" );
            iterator = m_sockets_slave.erase( iterator );
            if( m_sockets_slave.end( ) == iterator )
               break;
         }
         else if( os::Socket::eResult::OK == result )
         {
            size_t recv_size = 0;
            const void* const p_buffer = (*iterator)->buffer( recv_size );
            dsi::tByteStream stream;
            stream.push( p_buffer, recv_size );

            while( 0 < stream.size( ) )
            {
               dsi::Packet packet;
               stream.pop( packet );
               process_packet( packet, *(*iterator) );
            }
         }
      }

      // process client sockets
      for( auto iterator = m_sockets_client.begin( ); iterator != m_sockets_client.end( ); ++iterator )
      {
         if( false == fd_set.is_set( (*iterator)->socket( ), os::linux::socket::fd::eType::READ ) )
            continue;

         const os::Socket::eResult result = (*iterator)->recv( );
         if( os::Socket::eResult::DISCONNECTED == result )
         {
            (*iterator)->info( "Host disconnected" );
            iterator = m_sockets_client.erase( iterator );
            if( m_sockets_client.end( ) == iterator )
               break;
         }
         else if( os::Socket::eResult::OK == result )
         {
            size_t recv_size = 0;
            const void* const p_buffer = (*iterator)->buffer( recv_size );
            dsi::tByteStream stream;
            stream.push( p_buffer, recv_size );

            while( 0 < stream.size( ) )
            {
               dsi::Packet packet;
               stream.pop( packet );
               process_packet( packet, *(*iterator) );
            }
         }
      }

      // process master sockets
      if( true == fd_set.is_set( m_socket_master.socket( ), os::linux::socket::fd::eType::READ ) )
      {
         if( auto p_socket = m_socket_master.accept( ) )
         {
            m_sockets_slave.push_back( p_socket );
            p_socket->info( "Host connected" );
            p_socket->unblock( );
         }
      }
   }

   SYS_INF( "'%s': exit", m_name.c_str( ) );
}

bool ServiceIpcThread::start( )
{
   if( false == setup_connection( ) )
      return false;

   bool result_send = start_send( );
   bool result_receive = start_receive( );

   return result_send && result_receive;
}

bool ServiceIpcThread::setup_connection( )
{
   if( os::Socket::eResult::ERROR == m_socket_sb.create( ) )
      return false;
   if( os::Socket::eResult::ERROR == m_socket_sb.connect( ) )
      return false;
   m_socket_sb.unblock( );
   m_socket_sb.info( "ServiceBrocker connection created" );

   if( os::Socket::eResult::ERROR == m_socket_master.create( ) )
      return false;
   if( os::Socket::eResult::ERROR == m_socket_master.bind( ) )
      return false;
   m_socket_master.unblock( );
   if( os::Socket::eResult::ERROR == m_socket_master.listen( ) )
      return false;
   m_socket_master.info( "Application connection created" );

   return true;
}

bool ServiceIpcThread::start_send( )
{
   SYS_INF( "'%s': starting send thread", m_name.c_str( ) );
   if( false == mp_thread_send->run( ) )
   {
      SYS_ERR( "'%s': send thread can't be started", m_name.c_str( ) );
      return false;
   }
   return true;
}

bool ServiceIpcThread::start_receive( )
{
   SYS_INF( "'%s': starting recieve thread", m_name.c_str( ) );
   if( false == mp_thread_receive->run( ) )
   {
      SYS_ERR( "'%s': receive thread can't be started", m_name.c_str( ) );
      return false;
   }
   return true;
}

void ServiceIpcThread::stop( )
{
   stop_send( );
   stop_receive( );
}

void ServiceIpcThread::stop_send( )
{
   SYS_INF( "'%s': stopping send thread", m_name.c_str( ) );
   m_started_send = false;   
}

void ServiceIpcThread::stop_receive( )
{
   SYS_INF( "'%s': stopping receive thread", m_name.c_str( ) );
   m_started_receive = false;
}

void ServiceIpcThread::boot( const std::string& message )
{
   SYS_INF( "'%s': booting", m_name.c_str( ) );
}

void ServiceIpcThread::shutdown( const std::string& message )
{
   SYS_INF( "'%s': shutting down", m_name.c_str( ) );
   stop( );
}

bool ServiceIpcThread::insert_event( const base::async::IAsync::tSptr p_event )
{
   if( false == started( ) )
   {
      SYS_WRN( "'%s': is not started", m_name.c_str( ) );
      return false;
   }

   SYS_TRC( "'%s': inserting event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
   m_buffer_cond_var.lock( );
   m_events.push_back( p_event );
   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );

   return true;
}

base::async::IAsync::tSptr ServiceIpcThread::get_event( )
{
   m_buffer_cond_var.lock( );
   if( true == m_events.empty( ) )
   {
      SYS_TRC( "'%s': waiting for event...", m_name.c_str( ) );
      m_buffer_cond_var.wait( );
   }
   base::async::IAsync::tSptr p_event = m_events.front( );
   m_events.pop_front( );
   ++m_processed_events;
   SYS_TRC( "'%s': received event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
   m_buffer_cond_var.unlock( );

   return p_event;
}

void ServiceIpcThread::notify( const base::async::IAsync::tSptr p_event )
{
   // Processing runnable IAsync object
   if( base::async::eAsyncType::RUNNABLE == p_event->signature( )->type( ) )
   {
      m_process_started = time( nullptr );
      SYS_TRC( "'%s': start processing runnable at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
      p_event->process( );
      SYS_TRC( "'%s': finished processing runnable started at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
      m_process_started.reset( );
      return;
   }

   // Processing event IAsync object
   const auto& iterator = m_event_consumers_map.find( p_event->signature( ) );
   if( iterator == m_event_consumers_map.end( ) )
   {
      // In case if consumer was not found for event this means that this event must be sent via IPC.
      SYS_TRC( "'%s': sending event (%s) via IPC", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
      send( m_socket_sb, p_event );
   }
   else
   {
      // Here we create a copy of consumers set to avoid modifying consumers set during iterating it,
      // for example, during calling "clear_notification" from "process_event".
      auto consumers_set = iterator->second;
      SYS_TRC( "'%s': %zu consumers will be processed", m_name.c_str( ), consumers_set.size( ) );
      for( base::async::IAsync::IConsumer* p_consumer : consumers_set )
      {
         m_process_started = time( nullptr );
         SYS_TRC( "'%s': start processing event at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
         p_event->process( p_consumer );
         SYS_TRC( "'%s': finished processing event started at %ld (%s)", m_name.c_str( ), m_process_started.value( ), p_event->signature( )->name( ).c_str( ) );
      }
   }

   m_process_started.reset( );
}

void ServiceIpcThread::set_notification( const base::async::IAsync::ISignature& signature, base::async::IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   SYS_INF( "'%s': event id (%s) / consumer (%p)", m_name.c_str( ), signature.name( ).c_str( ), p_consumer );
   const auto& iterator = m_event_consumers_map.find( &signature );
   if( iterator == m_event_consumers_map.end( ) )
   {
      // Here signature is a temporary object created on a stack in concrete event static function.
      // So we should create a copy of this signature in a heap and store its pointer to consumers map.
      // Later on when number of consumers for this signature becase zero we must delete object by this pointer and remove poinetr from this map.
      auto p_signature = signature.create_copy( );
      m_event_consumers_map.emplace(
         std::pair< const base::async::IAsync::ISignature*, std::set< base::async::IAsync::IConsumer* > >( p_signature, { p_consumer } )
      );
   }
   else
   {
      iterator->second.emplace( p_consumer );
   }
}

void ServiceIpcThread::clear_notification( const base::async::IAsync::ISignature& signature, base::async::IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   const auto& iterator = m_event_consumers_map.find( &signature );
   if( iterator == m_event_consumers_map.end( ) )
      return;

   iterator->second.erase( p_consumer );

   if( true == iterator->second.empty( ) )
   {
      // Number of events for this event signature is zero (last one has just deleted), so we should remove pointer to this signature from the map
      // and must delete dynamicly created object (in function "set_notification")
      delete iterator->first;
      m_event_consumers_map.erase( iterator );
   }
}

void ServiceIpcThread::clear_all_notifications( const base::async::IAsync::ISignature& signature, base::async::IAsync::IConsumer* p_consumer )
{
   if( nullptr == p_consumer ) return;

   for( auto iterator_map = m_event_consumers_map.begin( ); iterator_map != m_event_consumers_map.end( ); ++iterator_map )
   {
      if( signature.type_id( ) != iterator_map->first->type_id( ) )
         continue;

      auto& consumers_set = iterator_map->second;
      auto iterator_set = consumers_set.find( p_consumer );
      if( consumers_set.end( ) == iterator_set )
         continue;

      consumers_set.erase( iterator_set );

      if( true == consumers_set.empty( ) )
      {
         // Number of events for this event signature is zero (last one has just deleted), so we should remove pointer to this signature from the map
         // and must delete dynamicly created object (in function "set_notification")
         delete iterator_map->first;
         m_event_consumers_map.erase( iterator_map );
      }

      break;
   }
}

bool ServiceIpcThread::is_subscribed( const base::async::IAsync::tSptr p_event )
{
   if( base::async::eAsyncType::RUNNABLE == p_event->signature( )->type( ) )
      return true;

   return m_event_consumers_map.end( ) != m_event_consumers_map.find( p_event->signature( ) );
}

bool ServiceIpcThread::send( os::Socket& _socket, dsi::tByteStream& stream )
{
   size_t size = 0;
   const void* buffer = nullptr;
   if( false == stream.is_linear( buffer, size ) )
   {
      SYS_WRN( "stream is not linear" );
      return false;
   }

   const bool result = os::Socket::eResult::OK == _socket.send( buffer, size );
   stream.erase( size );
   return result;
}

bool ServiceIpcThread::send( dsi::tByteStream& stream )
{
   return send( m_socket_sb, stream );
}

bool ServiceIpcThread::send( os::Socket& _socket, const base::async::IAsync::tSptr p_event )
{
   dsi::Packet packet;
   packet.add_package( dsi::eCommand::BroadcastEvent, *(std::static_pointer_cast< base::async::IEvent >( p_event )) );
   dsi::tByteStream stream;
   stream.push( packet );

   return send( _socket, stream );
}

bool ServiceIpcThread::send( const base::async::IAsync::tSptr p_event )
{
   return send( m_socket_sb, p_event );
}

bool ServiceIpcThread::process_packet( dsi::Packet& packet, os::Socket& socket )
{
   bool result = true;
   for( dsi::Package& package : packet.packages( ) )
      result &= process_package( package, socket );

   return result;
}

bool ServiceIpcThread::process_package( dsi::Package& package, os::Socket& socket )
{
   SYS_TRC( "Processing package '%s'", package.c_str( ) );

   switch( package.command( ) )
   {
      case dsi::eCommand::BroadcastEvent:
      {
         base::async::IEvent::tSptr p_event = base::async::IEvent::deserialize( package.data( ) );
         if( nullptr == p_event )
         {
            SYS_ERR( "'%s': lost received event", m_name.c_str( ) );
         }
         else
         {
            SYS_TRC( "'%s': received event (%s)", m_name.c_str( ), p_event->signature( )->name( ).c_str( ) );
            p_event->send( base::async::eCommType::ITC );
         }
         break;
      }
      case dsi::eCommand::DetectedServer:
      {
         interface::Signature service_signature;
         const void* ptr = nullptr;
         std::string address;
         int port;
         if( false == package.data( service_signature, ptr, address, port ) )
         {
            SYS_ERR( "parce package error" );
            break;
         }
         SYS_INF( "Detected server for service signature: '%s' (%p) (%s:%d)", service_signature.name( ).c_str( ), ptr, address.c_str( ), port );

         os::Socket::tSptr p_client_socket =
                  std::make_shared< os::Socket >(
                        os::linux::socket::configuration{ AF_UNIX, SOCK_STREAM, 0, address, port },
                        ServiceProcess::instance( )->configuration( ).ipc_app_buffer_size
                     );
         if( os::Socket::eResult::ERROR == p_client_socket->create( ) )
            break;
         if( os::Socket::eResult::ERROR == p_client_socket->connect( ) )
            break;
         p_client_socket->unblock( );
         p_client_socket->info( "Client connection created" );
         m_sockets_client.push_back( p_client_socket );

         const auto& clients = ServiceProcess::instance( )->connection_db( ).clients( service_signature );
         for( const auto& client : clients )
         {
            dsi::Packet packet;
            packet.add_package(
                  dsi::eCommand::DetectedClient, service_signature, client.ptr( ),
                  ServiceProcess::instance( )->configuration( ).ipc_app.address,
                  ServiceProcess::instance( )->configuration( ).ipc_app.port
               );
            dsi::tByteStream stream;
            stream.push( packet );
            send( *p_client_socket, stream );
         }

         auto result = ServiceProcess::instance( )->connection_db( ).register_server(
               service_signature, base::interface::Address( ptr, p_client_socket->id( ) )
            );

         break;
      }
      case dsi::eCommand::DetectedClient:
      {
         interface::Signature service_signature;
         const void* ptr = nullptr;
         std::string address;
         int port;
         if( false == package.data( service_signature, ptr, address, port ) )
         {
            SYS_ERR( "parce package error" );
            break;
         }
         SYS_INF( "Detected client for service signature: '%s' (%p) (%s:%d)", service_signature.name( ).c_str( ), ptr, address.c_str( ), port );

         auto result = ServiceProcess::instance( )->connection_db( ).register_client(
               service_signature, base::interface::Address( ptr, socket.id( ) )
            );

         break;
      }
      case dsi::eCommand::RegisterServer:
      case dsi::eCommand::UnregisterServer:
      case dsi::eCommand::RegisterClient:
      case dsi::eCommand::UnregisterClient:
      default:
      {
         SYS_WRN( "Unknown package command" );
         break;
      }
   }

   return true;
}
