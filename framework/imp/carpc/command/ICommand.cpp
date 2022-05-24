#include "carpc/command/ICommand.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "ICommand"



using namespace carpc::command;




const char* ICommand::c_str( const eAction& action )
{
   switch( action )
   {
      case eAction::EXECUTE:  return "EXECUTE";
      case eAction::PAUSE:    return "PAUSE";
      case eAction::RESUME:   return "RESUME";
      case eAction::STOP:     return "STOP";
      default:                return "UNEFINED";
   }
   return "UNDEFINED";
}

ICommand::ICommand( )
{
   SYS_DBG( "'%s': created", m_id.name( ).c_str( ) );
}

ICommand::~ICommand( )
{
   SYS_DBG( "'%s': destroyed", m_id.name( ).c_str( ) );
}

bool ICommand::state( const command::eState& new_state )
{
   bool result = false;
   switch( new_state )
   {
      case command::eState::IDLE:
      {
         break;
      }
      case command::eState::EXECUTING:
      {
         result = command::eState::IDLE == m_state || command::eState::RESUMING == m_state;
         break;
      }
      case command::eState::PAUSING:
      {
         result = command::eState::EXECUTING == m_state;
         break;
      }
      case command::eState::PAUSED:
      {
         result = command::eState::PAUSING == m_state;
         break;
      }
      case command::eState::RESUMING:
      {
         result = command::eState::PAUSED == m_state;
         break;
      }
      case command::eState::STOPPING:
      {
         result = command::eState::EXECUTING == m_state || command::eState::PAUSED == m_state;
         break;
      }
      case command::eState::STOPED:
      {
         result = command::eState::STOPPING == m_state;
         break;
      }
      case command::eState::FINISHED:
      {
         result = command::eState::EXECUTING == m_state;
         break;
      }
   }

   if( true == result )
   {
      SYS_DBG( "'%s': '%s' -> '%s'", m_id.name( ).c_str( ), command::c_str( m_state ), command::c_str( new_state )  );
      m_state = new_state;
      Signal::Event::create_send( { m_parent_id }, { m_id, m_state } );
   }
   else
   {
      SYS_ERR( "'%s': '%s' -> '%s'", m_id.name( ).c_str( ), command::c_str( m_state ), command::c_str( new_state )  );
   }

   return result;
}

void ICommand::action( const eAction& act )
{
   switch( act )
   {
      case eAction::EXECUTE:
      {
         if( false == state( command::eState::EXECUTING ) )
            break;

         execute(
            callback::tBlockerRoot::create(
               [ wp = tWptr( shared_from_this( ) ), id = m_id ]( )
               {
                  if( auto p = wp.lock( ) ) p->state( command::eState::FINISHED );
                  else SYS_ERR( "'%s': command was deleted", id.name( ).c_str( ) );
               }
            )
         );

         break;
      }
      case eAction::PAUSE:
      {
         if( false == state( command::eState::PAUSING ) )
            break;

         pause(
            callback::tBlockerRoot::create(
               [ wp = tWptr( shared_from_this( ) ), id = m_id ]( )
               {
                  if( auto p = wp.lock( ) ) p->state( command::eState::PAUSED );
                  else SYS_ERR( "'%s': command was deleted", id.name( ).c_str( ) );
               }
            )
         );

         break;
      }
      case eAction::RESUME:
      {
         if( false == state( command::eState::RESUMING ) )
            break;

         resume(
            callback::tBlockerRoot::create(
               [ wp = tWptr( shared_from_this( ) ), id = m_id ]( )
               {
                  if( auto p = wp.lock( ) ) p->state( command::eState::EXECUTING );
                  else SYS_ERR( "'%s': command was deleted", id.name( ).c_str( ) );
               }
            )
         );

         break;
      }
      case eAction::STOP:
      {
         if( false == state( command::eState::STOPPING ) )
            break;

         stop(
            callback::tBlockerRoot::create(
               [ wp = tWptr( shared_from_this( ) ), id = m_id ]( )
               {
                  if( auto p = wp.lock( ) ) p->state( command::eState::STOPED );
                  else SYS_ERR( "'%s': command was deleted", id.name( ).c_str( ) );
               }
            )
         );

         break;
      }
      default:
      {
         SYS_ERR( "'%s': unhandled action: '%s'", m_id.name( ).c_str( ), c_str( act ) );
         break;
      }
   }
}
