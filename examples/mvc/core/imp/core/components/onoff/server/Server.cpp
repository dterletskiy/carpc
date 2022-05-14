// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServer"



using namespace core::components::onoff;



Server::Server( )
   : service::onoff::V2_0_0::Server( "OnOff_Core" )
{
   MSG_DBG( "created" );
   current_state( "Unloaded" );
}

Server::~Server( )
{
   MSG_DBG( "destroyed" );
}

void Server::connected( )
{
   MSG_DBG( "connected" );
}

void Server::disconnected( )
{
   MSG_DBG( "disconnected" );
}

void Server::request_start( )
{
   MSG_DBG( );
}

void Server::request_trigger_state( const std::string& state, const std::size_t& delay )
{
   MSG_DBG( "state: %s / delay: %zu", state.c_str( ), delay );

   std::shared_ptr< carpc::timer::Timer > timer = std::make_shared< carpc::timer::Timer >( this );
   timer->start( delay, 1 );
   m_timers.emplace_back( TimerSeqID{ state, timer, unblock_request( ) } );
   // m_timers.emplace_back( TimerSeqID{ state, timer, 0 } );
}

void Server::process_timer( const carpc::comm::timer::ID id )
{
   MSG_DBG( "timer '%s' expired", id.name( ).c_str( ) );

   auto iterator = std::find_if( m_timers.begin( ), m_timers.end( ), [ id ]( const TimerSeqID& element ){ return element.timer->id( ) == id; } );
   if( m_timers.end( ) == iterator )
      return;

   prepare_response( iterator->seq_id );
   response_trigger_state( true );
   current_state( iterator->current_state );
   m_timers.erase( iterator );
}
