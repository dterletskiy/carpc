// Framework
#include "api/sys/comm/event/Runnable.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"
#include "imp/app/components/OnOff/Component.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace application::components::onoff {



namespace {

   template< typename T >
   class Test
   {
      using tParametersVector = std::vector< T >;
      using tParameter = typename tParametersVector::iterator;
      using tTestFunction = std::function< void( const T ) >;
   public:
      Test( tTestFunction _function, const tParametersVector& _parameters, const size_t _count )
         : m_parameters( _parameters )
         , m_parameter( m_parameters.begin( ) )
         , m_function( _function )
         , m_count( _count )
         , m_iteration( 0 )
         , m_performance( "Send Event" )
      { }

      void init( const size_t _count )
      {
         reset( );
         m_count = _count;
         DBG_WRN( "Test initialized (%zu)", m_count );
      }
      void reset( )
      {
         m_performance.stop( "Reset test" );
         m_iteration = 0;
         m_parameter = m_parameters.begin( );
      }
      bool execute( )
      {
         if( 0 == m_iteration )
         {
            m_performance.start( base::format_string( "Sending ", m_count, " ", base::async::c_str( *m_parameter ), " events..." ) );
         }

         if( m_count >= ++m_iteration )
         {
            // DBG_TRC( "iteration: %zu", m_iteration );
            m_function( *m_parameter );
            return true;
         }

         m_performance.stop( base::format_string( "... done ", m_count, " ", base::async::c_str( *m_parameter ), " events" ) );
         DBG_MSG( "microseconds per event: %f", static_cast< float >( m_performance.info( ) ) / m_count );

         if( m_parameters.end( ) == ++m_parameter )
         {
            DBG_WRN( "Test finished (%zu)", m_count );
            return false;
         }

         m_iteration = 0;
         return execute( );
      }
      const size_t count( ) const { return m_count; }
      const size_t iteration( ) const { return m_iteration; }
      const T parameter( ) const { return *m_parameter; }

   private:
      tParametersVector    m_parameters;
      tParameter           m_parameter;
      tTestFunction        m_function;
      size_t               m_count;
      size_t               m_iteration;
      base::tools::Performance   m_performance;
   };
   static size_t s_count = 1000000;
   auto send_event = [ ]( const base::async::eCommType _type )
   {
      events::AppEvent::Event::create_send( events::eAppEventID::PING, { base::async::c_str( _type ) }, _type );
   };
   Test< base::async::eCommType > s_event_test(
         send_event, { base::async::eCommType::ETC, base::async::eCommType::ITC, base::async::eCommType::IPC }, s_count
      );

}



base::IComponent::tSptr Component::creator( base::IServiceThread& service )
{
   return std::shared_ptr< Component >( new Component( service, "OnOff" ) );
}

Component::Component( base::IServiceThread& service, const std::string& name )
   : base::RootComponent( service, name )
   , m_server_onoff( "OnOffService", "OnOffService-Server" )
   , m_timer( this )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   // sleep(3);

   const base::ID id = base::timer::start( 3000, 1, [ this ]( const base::ID id ){ on_timer( id ); } );
   DBG_MSG( "started timer: %zu", id );
}

void Component::process_timer( const base::ID id )
{
   DBG_MSG( "Timer '%zu' expired", id );
   if( id == m_timer.id( ) )
   {
      DBG_WRN( "Shutting down system" );
      shutdown( );
   }
}

void Component::on_timer( const base::ID id )
{
   DBG_MSG( "Timer expired: %zu", id );

   // s_event_test.execute( );

   auto operation = [ this ]( )
   {
      DBG_MSG( "operation" );
      events::AppEvent::Event::create_send( { events::eAppEventID::BOOT }, { "booting" }, base::async::eCommType::ITC );
      m_timer.start( 15000000000 );
   };
   base::async::Runnable::create_send( operation );
}



} // namespace application::components::onoff
