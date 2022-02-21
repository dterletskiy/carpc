// Application
#include "SomeIP.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SomeIP_Client"



using namespace symbiont::interface::someip;



Client::Client( )
   : m_app( vsomeip::runtime::get( )->create_application( ) )
{
   MSG_VRB( "created" );
}

Client::~Client( )
{
   MSG_VRB( "destroyed" );
}


bool Client::init( )
{
   if( false == m_app->init( ) )
   {
      MSG_ERR( "Couldn't initialize some/ip application" );
      return false;
   }

   m_app->register_state_handler( std::bind( &Client::on_state, this, std::placeholders::_1) );

   m_app->register_message_handler(
      vsomeip::ANY_SERVICE, SAMPLE_INSTANCE_ID, vsomeip::ANY_METHOD,
      std::bind( &Client::on_message, this, std::placeholders::_1 )
   );

   m_app->register_availability_handler(
      SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID,
      std::bind(
         &Client::on_availability, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
      )
   );

   return true;
}

void Client::start( )
{
   m_app->start( );
}

void Client::stop( )
{
   m_app->clear_all_handler( );
   m_app->release_service( SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID );
   m_app->stop( );
}

void Client::on_state( vsomeip::state_type_e state )
{
   MSG_DBG( "Application '%s' is %s", m_app->get_name( ).c_str( ), vsomeip::state_type_e::ST_REGISTERED == state ? "registered." : "deregistered." );
   m_is_registered = vsomeip::state_type_e::ST_REGISTERED == state;

   if( vsomeip::state_type_e::ST_REGISTERED == state )
   {
      m_app->request_service( SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID );
   }
}

void Client::on_availability( vsomeip::service_t service, vsomeip::instance_t instance, bool is_connected )
{
   MSG_DBG( "Service %x.%x is %savailable", service, instance, is_connected ? "" : "not " );

   if( SAMPLE_SERVICE_ID == service && SAMPLE_INSTANCE_ID == instance )
   {
      if( m_is_connected  && !is_connected )
      {
         m_is_connected = false;
      }
      else if( is_connected && !m_is_connected )
      {
         m_is_connected = true;
         test( );
      }
   }
}

void Client::on_message( const std::shared_ptr< vsomeip::message >& message )
{
   test( );
}

void Client::request( const std::string& state, const std::size_t delay )
{
   std::shared_ptr< vsomeip::message > message = vsomeip::runtime::get( )->create_request( false /* tcp/ip or udp/ip */ );

   message->set_service( SAMPLE_SERVICE_ID );
   message->set_instance( SAMPLE_INSTANCE_ID );
   message->set_method( SAMPLE_METHOD_ID );

   std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get( )->create_payload( );
   std::vector< vsomeip::byte_t > payload_data;
   for( std::size_t i = 0; i < state.size( ); ++i )
      payload_data.push_back( i );
   payload_data.push_back( delay );
   payload->set_data( payload_data );
   message->set_payload( payload );

   m_app->send( message );
}

void Client::run_test( )
{
   m_is_test = true;
   test( );
}

void Client::test( )
{
   if( !m_is_connected || !m_is_test )
   {
      MSG_WRN( "connected: %d / tested: %d", m_is_connected, m_is_test );
      return;
   }

   static std::size_t count = 0;

   if( 0 == count )
   {
      MSG_DBG( "TEST STARTED SOMEIP" );
      m_performance.start( );
   }

   if( 1000000 > ++count )
   {
      request( "REQUEST_TEST", 0 );
   }
   else
   {
      m_performance.stop( );
      MSG_DBG( "TEST FINISHED SOMEIP: count = %zu", count );
   }
}
