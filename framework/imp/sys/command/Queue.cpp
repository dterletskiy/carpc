#include "api/sys/command/Queue.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "CommandQueue"



using namespace carpc::command;



Queue::Queue( const std::string& name )
   : m_name( name )
{
   SYS_DBG( "'%s': created '%s'", m_name.c_str( ), m_id.name( ).c_str( ) );
   ICommand::Signal::Event::set_notification( this, { m_id } );
}

Queue::~Queue( )
{
   SYS_DBG( "'%s': destroyed '%s'", m_name.c_str( ), m_id.name( ).c_str( ) );
   ICommand::Signal::Event::clear_all_notifications( this );
}

bool Queue::state( const queue::eState& new_state )
{
   bool result = false;
   switch( new_state )
   {
      case queue::eState::IDLE:
      {
         result = queue::eState::RUNNING == m_state;
         break;
      }
      case queue::eState::RUNNING:
      {
         result = queue::eState::IDLE == m_state || queue::eState::RESUMING == m_state;
         break;
      }
      case queue::eState::PAUSING:
         result = queue::eState::RUNNING == m_state;
      {
         break;
      }
      case queue::eState::PAUSED:
         result = queue::eState::PAUSING == m_state;
      {
         break;
      }
      case queue::eState::RESUMING:
         result = queue::eState::PAUSED == m_state;
      {
         break;
      }
      case queue::eState::STOPPING:
      {
         result = queue::eState::PAUSED == m_state || queue::eState::RUNNING == m_state;
         break;
      }
      case queue::eState::STOPED:
      {
         result = queue::eState::STOPED == m_state;
         break;
      }
   }

   if( true == result )
   {
      SYS_DBG( "'%s': %s' -> '%s'", m_name.c_str( ), queue::c_str( m_state ), queue::c_str( new_state )  );
      m_state = new_state;
   }
   else
   {
      SYS_ERR( "'%s': %s' -> '%s'", m_name.c_str( ), queue::c_str( m_state ), queue::c_str( new_state )  );
   }

   return result;
}

bool Queue::start( )
{
   if( false == state( queue::eState::RUNNING ) )
      return false;

   execute_next( );
   return true;
}

bool Queue::stop( )
{
   if( false == state( queue::eState::STOPPING ) )
      return false;

   m_commands.front( )->action( ICommand::eAction::STOP );
   return true;
}

bool Queue::pause( )
{
   if( false == state( queue::eState::PAUSING ) )
      return false;

   m_commands.front( )->action( ICommand::eAction::PAUSE );
   return true;
}

bool Queue::resume( )
{
   if( false == state( queue::eState::RESUMING ) )
      return false;

   m_commands.front( )->action( ICommand::eAction::RESUME );
   return true;
}

bool Queue::execute_next( )
{
   if( queue::eState::RUNNING != m_state )
      return false;

   if( m_commands.empty( ) )
      return false;

   if( command::eState::EXECUTING == m_commands.front( )->state( ) )
      return false;

   SYS_DBG( "'%s': starting next command: %s", m_name.c_str( ), m_commands.front( )->id( ).name( ).c_str( ) );
   m_commands.front( )->action( ICommand::eAction::EXECUTE );

   return true;
}

void Queue::process_event( const typename ICommand::Signal::Event& event )
{
   if( nullptr == event.data( ) )
      return;

   const auto& data = *( event.data( ) );

   if( data.id != m_commands.front( )->id( ) )
   {
      SYS_WRN( "'%s': finished command that should not been running: %s", m_name.c_str( ), data.id.name( ).c_str( ) );
      return;
   }

   switch( data.state )
   {
      case command::eState::FINISHED:
      {
         // If Queue is in state PAUSING/STOPPING it is waiting for state PAUSED/STOPED from currently running state.
         // But could be a situation when running command finishes it executing and emit event with state FINISHED
         // before PAUSED/STOPED. In this case command is finished and Queue can trasit to required state without
         // starting execution next command.
         if( queue::eState::PAUSING == m_state )
            state( queue::eState::PAUSED );
         else if( queue::eState::STOPPING == m_state )
            state( queue::eState::STOPED );

         m_commands.pop_front( );
         execute_next( );

         break;
      }
      case command::eState::EXECUTING: // RESUMED
      {
         state( queue::eState::RUNNING );
         execute_next( );
         break;
      }
      case command::eState::PAUSED:
      {
         state( queue::eState::PAUSED );
         break;
      }
      case command::eState::STOPED:
      {
         state( queue::eState::STOPED );
         break;
      }
      default:
      {
         SYS_WRN( "'%s': unhandled command state '%s' in queue", m_name.c_str( ), command::c_str( data.state ) );
         break;
      }
   }
}
