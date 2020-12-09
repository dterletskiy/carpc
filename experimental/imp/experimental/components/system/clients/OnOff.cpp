#include "imp/experimental/components/system/clients/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff_Client"



using namespace component::system::clients;



OnOff::OnOff( )
   : api::onoff::Proxy( "Player" )
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
   request_boot( "Booting" );
}

void OnOff::disconnected( )
{
   DBG_MSG( );
}

void OnOff::response_boot( const bool result )
{
   DBG_MSG( "result: %s", result ? "true" : "false" );
}
