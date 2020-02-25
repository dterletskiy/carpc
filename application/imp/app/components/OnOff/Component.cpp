// Application
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
            m_performance.start( base::format_string( "Sending ", m_count, " ", base::c_str( *m_parameter ), " events..." ) );
         }

         if( m_count >= ++m_iteration )
         {
            // DBG_TRC( "iteration: %zu", m_iteration );
            m_function( *m_parameter );
            return true;
         }

         m_performance.stop( base::format_string( "... done ", m_count, " ", base::c_str( *m_parameter ), " events" ) );
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
   auto send_event = [ ]( const base::eCommType _type ) { events::IPC::PingEvent::Event::create_send( events::eEventID::ping, { base::c_str( _type ) }, _type ); };
   Test< base::eCommType > s_event_test( send_event, { base::eCommType::ETC, base::eCommType::ITC, base::eCommType::IPC }, s_count );

}



base::ComponentPtr Component::creator( base::ServiceThreadPtr p_service )
{
   return std::shared_ptr< Component >( new Component( p_service, "OnOff" ) );
}

Component::Component( const base::ServiceThreadPtr p_service, const std::string& name )
   : base::RootComponent( p_service, name )
   , m_server_onoff( "OnOffService", "OnOffService-Server" )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   // events::NoID::PingEvent::Event::set_notification( this );
   // events::ID::PingEvent::Event::set_notification( this, events::eEventID::boot );
   // events::ID::PingEvent::Event::set_notification( this, events::eEventID::ping );
   // events::IPC::PingEvent::Event::set_notification( this, events::eEventID::ping );
}

Component::~Component( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   // events::NoID::PingEvent::Event::clear_notification( this );
   // events::ID::PingEvent::Event::clear_notification( this, events::eEventID::boot );
   // events::ID::PingEvent::Event::clear_notification( this, events::eEventID::ping );
   // events::IPC::PingEvent::Event::clear_notification( this, events::eEventID::ping );
}

bool Component::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );

   // s_event_test.execute( );

   // events::NoID::PingEvent::Event::create_send( { "WTF!!!" } );
   events::ID::PingEvent::Event::create_send( events::eEventID::boot, { "booting" } );
   // events::IPC::PingEvent::Event::create_send( events::eEventID::ping, { "WTF!!!" }, base::eCommType::IPC );

   return true;
}

void Component::process_event( const events::NoID::PingEvent::Event& event )
{
   DBG_MSG( "message = %s", event.data( )->message.c_str( ) );
}

void Component::process_event( const events::ID::PingEvent::Event& event )
{
   DBG_MSG( "message = %s", event.data( )->message.c_str( ) );
}

void Component::process_event( const events::IPC::PingEvent::Event& event )
{
   DBG_MSG( "message = %s", event.data( )->message.c_str( ) );

   if( false == s_event_test.execute( ) ) shutdown( );
}



} // namespace application::components::onoff
