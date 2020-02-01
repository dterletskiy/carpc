#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/tools/Performance.hpp"
// Application
#include "imp/app/components/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace application::onoff {



base::ComponentPtr creator( base::ServiceThreadPtr p_service )
{
   return std::make_shared< OnOff >( p_service, "OnOff" );
}



OnOff::OnOff( const base::ServiceThreadPtr p_service, const std::string& name )
   : base::RootComponent( p_service, name )
   , m_server_onoff_xxx( "xxx" )
   , m_client_onoff_xxx( "xxx" )
   , m_server_onoff_yyy( "yyy" )
   , m_client_onoff_yyy( "xxx" )
   , m_timer( this )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   events::NoID::PingEvent::Event::set_notification( this );
   events::ID::PingEvent::Event::set_notification( this, events::eEventID::request );
   events::IPC::PingEvent::Event::set_notification( this, events::eEventID::request );
}

OnOff::~OnOff( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   events::NoID::PingEvent::Event::clear_notification( this );
   events::ID::PingEvent::Event::clear_notification( this, events::eEventID::request );
   events::IPC::PingEvent::Event::clear_notification( this, events::eEventID::request );
}



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
   auto send_event = [ ]( const base::eCommType _type ) { events::IPC::PingEvent::Event::create_send( events::eEventID::request, { base::c_str( _type ) }, _type ); };
   // Test< base::eCommType > s_event_test( send_event, { base::eCommType::ETC, base::eCommType::ITC, base::eCommType::IPC }, s_count );
   Test< base::eCommType > s_event_test( send_event, { base::eCommType::ETC }, s_count );

}

bool OnOff::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   // sleep(5);

   // s_event_test.execute( );

   // m_client_onoff_xxx.request_trigger_state( "Unloaded", 5000000000 );
   // m_client_onoff_yyy.request_trigger_state( "BasicOperable", 10000000000 );

   // m_timer.start( 1000000000 );

   // events::NoID::PingEvent::Event::create_send( { "WTF!!!" } );
   // events::ID::PingEvent::Event::create_send( events::eEventID::request, { "WTF!!!" } );
   // events::IPC::PingEvent::Event::create_send( events::eEventID::request, { "WTF!!!" }, base::eCommType::IPC );

   return true;
}

void OnOff::process_event( const events::NoID::PingEvent::Event& event )
{
   DBG_ERR( "info = %s", event.data( )->info.c_str( ) );
   // events::NoID::PingEvent::Event::clear_notification( this );
   // events::NoID::PingEvent::Event::set_notification( this );
   // events::NoID::PingEvent::Event::create_send( { "WTF!!!" } );
}

void OnOff::process_event( const events::ID::PingEvent::Event& event )
{
   DBG_ERR( "info = %s", event.data( )->info.c_str( ) );
   // events::ID::PingEvent::Event::clear_notification( this, events::eEventID::request );
   // events::ID::PingEvent::Event::set_notification( this, events::eEventID::request );
   // events::ID::PingEvent::Event::create_send( events::eEventID::request, { "WTF!!!" } );
}

void OnOff::process_event( const events::IPC::PingEvent::Event& event )
{
   // DBG_ERR( "info = %s", event.data( )->info.c_str( ) );
   // events::IPC::PingEvent::Event::clear_notification( this, events::eEventID::request );
   // events::IPC::PingEvent::Event::set_notification( this, events::eEventID::request );
   // events::IPC::PingEvent::Event::create_send( events::eEventID::request, { "WTF!!!" } );

   if( false == s_event_test.execute( ) ) shutdown( );
}

void OnOff::process_timer( const base::TimerID id )
{
   static size_t count = 0;
   DBG_TRC( "Timer '%#lx' expired '%zu' times", (long)id, ++count );
   DBG_TRC( "%s", BOOL_TO_STRING( m_timer.id( ) == id ) );

   if( 10 < count )
   {
      m_timer.stop( );
      m_timer.start( 1000000000 );
   }
}



} // namespace application::onoff
