// Application
#include "OnOff_v.1.0.0.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff_Server_Master_1.0.0"



using namespace application::components::master::server::V1_0_0;



OnOff::OnOff( const std::string& role_name )
   : service::onoff::V1_0_0::Server( role_name )
{
   MSG_DBG( "created" );
   current_state( "Unloaded" );
}

OnOff::~OnOff( )
{
   MSG_DBG( "destroyed" );
}

void OnOff::request_trigger_state( const std::string& state, const size_t delay )
{
   MSG_DBG( "state: %s / delay: %zu", state.c_str( ), delay );

   std::shared_ptr< carpc::timer::Timer > timer = std::make_shared< carpc::timer::Timer >( this, state );
   timer->start( delay, 1 );
   m_timers.emplace_back( TimerSeqID{ state, timer, unblock_request( ) } );
}

void OnOff::process_timer( const carpc::comm::timer::ID id )
{
   MSG_DBG( "Timer '%s' expired", id.name( ).c_str( ) );

   auto iterator = std::find_if( m_timers.begin( ), m_timers.end( ), [ id ]( const TimerSeqID& element ){ return element.timer->id( ) == id; } );
   if( m_timers.end( ) == iterator )
      return;

   prepare_response( iterator->seq_id );
   response_trigger_state( true );
   current_state( iterator->current_state );
   m_timers.erase( iterator );
}