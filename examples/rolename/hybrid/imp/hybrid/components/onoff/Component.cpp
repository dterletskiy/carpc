// Application
#include "imp/hybrid/components/onoff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



using namespace hybrid::components::onoff;



base::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "OnOff" ) );
}

Component::Component( const std::string& _name )
   : base::application::RootComponent( _name )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

#include <thread>
void Component::boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );

   // if( true == m_server_someip.init( ) )
   //    m_server_someip.start( );

   std::thread thread(
      [ this ]( )
      {
         if( m_server_someip.init( ) ) m_server_someip.start( );
      }
   );
   thread.detach( );
}
