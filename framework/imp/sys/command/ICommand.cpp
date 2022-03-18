#include "api/sys/command/ICommand.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "ICommand"



using namespace carpc::command;



ICommand::ICommand( )
{
   SYS_DBG( "id: %s", m_id.name( ).c_str( ) );
}

ICommand::~ICommand( )
{
   SYS_DBG( "id: %s", m_id.name( ).c_str( ) );
}

void ICommand::finish( )
{
   Signal::Event::create_send( { m_parent_id }, { m_id, eSignal::FINISH } );
}
