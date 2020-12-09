#include "imp/experimental/components/onoff/servers/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff_Server"



using namespace component::onoff::servers;



OnOff::OnOff( )
   : api::onoff::Stub( "Player" )
{
}

OnOff::~OnOff( )
{
}

std::shared_ptr< OnOff > OnOff::create( )
{
   return std::shared_ptr< OnOff >( new OnOff( ) );
}

void OnOff::connected( )
{
   DBG_MSG( );
}

void OnOff::disconnected( )
{
   DBG_MSG( );
}

void OnOff::request_boot( const std::string& message )
{
   DBG_MSG( "message: %s", message.c_str( ) );
   response_boot( true );
}
