#include "api/sys/helpers/functions/format.hpp"
#include "api/sys/tools/Performance.hpp"
// Application
#include "imp/app/components/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace application::onoff {



base::ComponentPtr creator( base::ServicePtr p_service )
{
   return std::make_shared< OnOff >( p_service, "OnOff" );
}



OnOff::OnOff( const base::ServicePtr p_service, const std::string& name )
   : base::RootComponent( p_service, name )
   , m_server( )
   , m_client( )
{
   DBG_MSG( "Created: %s", base::Component::name( ).c_str( ) );
   ServiceDSI::PingEvent::Event::set_notification( true, this );
}

OnOff::~OnOff( )
{
   DBG_MSG( "Destroyed: %s", name( ).c_str( ) );
   ServiceDSI::PingEvent::Event::set_notification( false, this );
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
            m_function( *m_parameter );
            return true;
         }

         m_performance.stop( base::format_string( "... done ", m_count, " ", base::c_str( *m_parameter ), " events" ) );

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
   static size_t s_count = 10;
   auto send_event = [ ]( const base::eCommType _type ) { ServiceDSI::PingEvent::Event::create_send( { base::c_str( _type ) }, _type ); };
   Test< base::eCommType > s_event_test( send_event, { base::eCommType::ETC, base::eCommType::ITC, base::eCommType::IPC }, s_count );

}

bool OnOff::boot( const std::string& command )
{
   DBG_MSG( "%s", command.c_str( ) );
   // sleep(5);
   m_client.request_trigger_state( "Unloaded" );
   s_event_test.execute( );

   return true;
}

void OnOff::process_event( const ServiceDSI::PingEvent::Event& event )
{
   // DBG_TRC( "info = %s", event.data( )->info.c_str( ) );
   bool result = s_event_test.execute( );
   if( !result )
   {
      s_count *= 10;
      if( 10 >= s_count )
      {
         s_event_test.init( s_count );
         s_event_test.execute( );
      }
      else
      {
         shutdown( );
      }
   }

   // DBG_TRC( "id = %#zx", static_cast< size_t >( event.id( ) ) );
   // switch( event.id( ) )
   // {
   //    case events::eEventID::request:
   //    {
   //       DBG_WRN( "request" );
   //       break;
   //    }
   //    case events::eEventID::response:
   //    {
   //       DBG_WRN( "response" );
   //       break;
   //    }
   //    default: break;
   // }
}



} // namespace application::onoff
