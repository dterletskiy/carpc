// Application
#include "Server.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOffServer"



using namespace application::components::onoff;



Server::Server( const std::string& role_name, const std::string& name )
   : base::onoff::Server( role_name )
   , m_name( name )
{
   MSG_DBG( "%s: created", m_name.c_str( ) );
   current_state( "Unloaded" );
}

Server::~Server( )
{
   MSG_DBG( "%s: destroyed", m_name.c_str( ) );
}

void Server::request_start( )
{
   MSG_DBG( );
}

void Server::request_trigger_state( const std::string& state, const size_t delay )
{
   MSG_DBG( "%s: state: %s / delay: %zu", m_name.c_str( ), state.c_str( ), delay );

   std::shared_ptr< base::timer::Timer > timer = std::make_shared< base::timer::Timer >( this, state );
   timer->start( delay, 1 );
   m_timers.emplace_back( TimerSeqID{ state, timer, unblock_request( ) } );
   // m_timers.emplace_back( TimerSeqID{ state, timer, 0 } );
}

void Server::process_timer( const base::comm::timer::ID id )
{
   MSG_DBG( "%s: Timer '%s' expired", m_name.c_str( ), id.name( ).c_str( ) );

   auto iterator = std::find_if( m_timers.begin( ), m_timers.end( ), [ id ]( const TimerSeqID& element ){ return element.timer->id( ) == id; } );
   if( m_timers.end( ) == iterator )
      return;

   prepare_response( iterator->seq_id );
   response_trigger_state( true );
   current_state( iterator->current_state );
   m_timers.erase( iterator );
}
