// Framework
#include "api/sys/comm/async/runnable/Runnable.hpp"
#include "api/sys/comm/async/callable/TCallable.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"
#include "imp/app/components/OnOff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



using namespace application::components::onoff;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "OnOff" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::RootComponent( _name )
   , m_server_onoff( "OnOffService", "OnOffService-Server" )
   , m_timer( this, _name )
   , m_fsm( )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
   m_fsm.run( );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::process_boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );
   // sleep(3);

   const carpc::comm::timer::ID id = carpc::timer::start( 3000, 1, [ this ]( const carpc::comm::timer::ID id ){ on_timer( id ); } );
   MSG_DBG( "started timer: %s", id.name( ).c_str( ) );
}

void Component::process_shutdown( carpc::callback::tBlocker blocker )
{
   auto operation = [ blocker ]( )
   {
      MSG_MARKER( "START" );
      sleep( 5 );
      MSG_MARKER( "END" );
   };
   carpc::async::Runnable::create_send( operation );
}

void Component::process_timer( const carpc::comm::timer::ID id )
{
   MSG_DBG( "Timer '%s' expired", id.name( ).c_str( ) );
   if( id == m_timer.id( ) )
   {
      MSG_WRN( "Shutting down system" );
      shutdown( );
   }
}

void Component::on_timer( const carpc::comm::timer::ID id )
{
   MSG_DBG( "Timer expired: %s", id.name( ).c_str( ) );

   #if 1
   {
      auto operation = [ this ]( )
      {
         MSG_DBG( "operation" );
         events::AppEvent::Event::create_send( { events::eAppEventID::BOOT }, { "booting" } );
         m_timer.start( 20000000000, 1 );
      };
      carpc::async::Runnable::create_send( operation );

      m_fsm.signal( { } );
   }
   #endif

   #if 0
   {
      auto operation_runnable = [ ]( )
      {
         MSG_MARKER( "operation_runnable START" );
         sleep( 5 );
         MSG_MARKER( "operation_runnable END" );
      };

      MSG_MARKER( "START" );
      carpc::async::Runnable::create_send(
            operation_runnable,
            carpc::application::Context( carpc::application::thread::id( "Driver_Service" ) ),
            true
         );
      MSG_MARKER( "END" );

      MSG_MARKER( "START" );
      carpc::async::Runnable::create_send(
            operation_runnable,
            carpc::application::Context( carpc::application::thread::current_id( ) ),
            true
         );
      MSG_MARKER( "END" );
   }
   #endif

   #if 0
   {
      std::function< void( const std::size_t ) > operation_params =
      [ ]( const std::size_t index )
      {
         MSG_MARKER( "operation_params START" );
         MSG_DBG( "index = %zu", index );
         sleep( 5 );
         MSG_MARKER( "operation_params END" );
      };

      MSG_MARKER( "START" );
      const std::size_t index = 12345;

      carpc::async::TCallable< const std::size_t >::create( operation_params, index )->send(
            carpc::application::Context( carpc::application::thread::id( "Driver_Service" ) )
         );

      carpc::async::callable::create( operation_params, index )->send(
            carpc::application::Context( carpc::application::thread::id( "Driver_Service" ) )
         );
      MSG_MARKER( "END" );
   }
   #endif
}
