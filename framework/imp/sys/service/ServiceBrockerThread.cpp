#include "api/sys/oswrappers/Socket.hpp"
#include "api/sys/configuration/DSI.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/service/ServiceBrockerThread.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SrvBr"



using namespace base;



ServiceBrockerThread::tSptr ServiceBrockerThread::mp_instance;

ServiceBrockerThread::ServiceBrockerThread( )
   : m_events( )
{
   mp_thread_send = std::make_shared< os::Thread >( std::bind( &ServiceBrockerThread::thread_loop_send, this ) );
   mp_thread_receive = std::make_shared< os::Thread >( std::bind( &ServiceBrockerThread::thread_loop_receive, this ) );

   SYS_TRC( "created" );
}

ServiceBrockerThread::~ServiceBrockerThread( )
{
   SYS_TRC( "destroyed" );
}

namespace { os::Mutex s_mutex; }
ServiceBrockerThread::tSptr ServiceBrockerThread::instance( )
{
   base::os::MutexAutoLocker locker( s_mutex );
   if( nullptr == mp_instance )
      mp_instance.reset( new ServiceBrockerThread( ) );

   return mp_instance;
}

bool ServiceBrockerThread::setup_connection( )
{
   m_master_socket = base::socket::socket( configuration::dsi::socket_family, configuration::dsi::socket_type, configuration::dsi::socket_protocole );
   if( -1 == m_master_socket )
      return false;

   if( false == base::socket::connect( m_master_socket, configuration::dsi::socket_family, configuration::dsi::server_address, configuration::dsi::server_port ) )
      return false;

   base::socket::info( m_master_socket, "Connection created" );

   return true;
}

bool ServiceBrockerThread::start( )
{
   if( false == setup_connection( ) )
      return false;

   SYS_INF( "Starting ServiceBrockerThread" );
   bool result_send = start_send( );
   bool result_receive = start_receive( );

   return result_send && result_receive;
}

void ServiceBrockerThread::stop( )
{
   SYS_INF( "Stopping ServiceBrockerThread" );
   stop_send( );
   stop_receive( );
}

void ServiceBrockerThread::thread_loop_send( )
{
   SYS_INF( "enter" );
   m_started_send = true;

   while( started_send( ) )
   {
      IEvent::tSptr p_event = get_event( );
      SYS_TRC( "sending event (%s)", p_event->signature( )->name( ).c_str( ) );
      ByteBufferT byte_buffer;
      if( false == EventRegistry::instance( )->create_buffer( byte_buffer, p_event ) )
         continue;
      // byte_buffer.dump( );

      base::socket::send( m_master_socket, byte_buffer.buffer( ), byte_buffer.size( ) );
   }

   SYS_INF( "exit" );
}

bool ServiceBrockerThread::start_send( )
{
   SYS_INF( "Starting ServiceBrockerThread send thread" );
   bool result = mp_thread_send->run( );
   if( false == result )
   {
      SYS_ERR( "ServiceBrockerThread can't start send thread" );
   }

   return result;
}

void ServiceBrockerThread::stop_send( )
{
   SYS_INF( "Stopping ServiceBrockerThread send thread" );
   m_started_send = false;
}

void ServiceBrockerThread::thread_loop_receive( )
{
   SYS_INF( "enter" );

   void* p_buffer = malloc( configuration::dsi::buffer_size );
   if( nullptr == p_buffer )
      return;

   m_started_receive = true;

   while( started_receive( ) )
   {
      memset( p_buffer, 0, sizeof( configuration::dsi::buffer_size ) );
      ssize_t recv_size = base::socket::recv( m_master_socket, p_buffer, configuration::dsi::buffer_size );
      if( 0 >= recv_size )
         continue;

      ByteBufferT byte_buffer( p_buffer, recv_size );
      // byte_buffer.dump( );

      while( 0 < byte_buffer.size( ) )
      {
         // @TDA: issue: in case of receiving several event all of them will be processed in reverce sequence
         IEvent::tSptr p_event = EventRegistry::instance( )->create_event( byte_buffer );
         if( nullptr == p_event )
         {
            SYS_ERR( "lost received event" );
            continue;
         }
         SYS_TRC( "received event (%s)", p_event->signature( )->name( ).c_str( ) );
         p_event->send( eCommType::ITC );
      }
   }

   free( p_buffer );
   SYS_INF( "exit" );
}

bool ServiceBrockerThread::start_receive( )
{
   SYS_INF( "Starting ServiceBrockerThread receive thread" );
   bool result = mp_thread_receive->run( );
   if( false == result )
   {
      SYS_ERR( "ServiceBrockerThread can't start receive thread" );
   }

   return result;
}

void ServiceBrockerThread::stop_receive( )
{
   SYS_INF( "Stopping ServiceBrockerThread receive thread" );
   m_started_receive = false;
}

bool ServiceBrockerThread::insert_event( const IEvent::tSptr p_event )
{
   if( false == m_started_send )
   {
      SYS_WRN( "ServiceBrockerThread send thread is not started" );
      return false;
   }

   m_buffer_cond_var.lock( );
   m_events.push_back( p_event );
   m_buffer_cond_var.notify( );
   m_buffer_cond_var.unlock( );
   return true;
}

IEvent::tSptr ServiceBrockerThread::get_event( )
{
   m_buffer_cond_var.lock( );
   if( true == m_events.empty( ) )
   {
      m_buffer_cond_var.wait( );
   }
   IEvent::tSptr p_event = m_events.front( );
   m_events.pop_front( );
   m_buffer_cond_var.unlock( );

   return p_event;
}
