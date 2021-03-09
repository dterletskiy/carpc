#include "api/sys/command/Queue.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "CommandQueue"



using namespace base::command;



Queue::Queue( const std::string& name )
   : m_name( name )
{
   SYS_DBG( "name: %s / id: %s", m_name.c_str( ), m_id.name( ).c_str( ) );
   ICommand::Signal::Event::set_notification( this, { m_id } );
}

Queue::~Queue( )
{
   SYS_DBG( "name: %s / id: %s", m_name.c_str( ), m_id.name( ).c_str( ) );
   ICommand::Signal::Event::clear_all_notifications( this );
}

bool Queue::start( )
{
   if( eState::IDLE != m_state )
      return false;

   m_state = eState::RUNNING;
   execute_next( );

   return true;
}

bool Queue::stop( )
{
   if( eState::RUNNING != m_state || eState::PAUSED != m_state )
      return false;

   m_state = eState::STOPPING;
   m_commands.front( )->stop( );
   for( auto iterator = m_commands.begin( ); iterator != m_commands.end( ); ++iterator )
      (*iterator)->finish( );

   return true;
}

bool Queue::pause( )
{
   if( eState::RUNNING != m_state )
      return false;

   m_state = eState::PAUSED;
   for( auto iterator = m_commands.begin( ); iterator != m_commands.end( ); ++iterator )
      (*iterator)->pause( );

   return true;
}

bool Queue::resume( )
{
   if( eState::PAUSED != m_state )
      return false;

   m_state = eState::RUNNING;
   for( auto iterator = m_commands.begin( ); iterator != m_commands.end( ); ++iterator )
      (*iterator)->resume( );

   return true;
}

bool Queue::execute_next( )
{
   if( eState::RUNNING != m_state )
      return false;

   if( m_commands.empty( ) )
      return false;

   SYS_DBG( "starting next command: %s", m_commands.front( )->id( ).name( ).c_str( ) );
   m_commands.front( )->execute( );

   return true;
}

void Queue::process_event( const typename ICommand::Signal::Event& event )
{
   if( nullptr == event.data( ) )
      return;

   const auto& data = *( event.data( ) );
   switch( data.signal )
   {
      case ICommand::eSignal::FINISH:
      {
         for( auto iterator = m_commands.begin( ); iterator != m_commands.end( ); ++iterator )
         {
            if( data.id != (*iterator)->id( ) )
               continue;

            if( m_commands.begin( ) != iterator )
            {
               SYS_WRN( "removing not running command: %s", (*iterator)->id( ).name( ).c_str( ) );
            }
            m_commands.erase( iterator );
            break;
         }

         if( eState::STOPPING == m_state )
            m_state = eState::STOPPED;
         else
            execute_next( );

         break;
      }
   }
}
