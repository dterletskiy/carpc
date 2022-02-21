// Application
#include "SomeIP.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "SomeIP_Server"



using namespace hybrid::interface::someip;



Server::Server( )
   : m_app( vsomeip::runtime::get( )->create_application( ) )
{
   MSG_VRB( "created" );
}

Server::~Server( )
{
   MSG_VRB( "destroyed" );
}


bool Server::init( )
{
   MSG_VRB( );

   if( false == m_app->init( ) )
   {
      MSG_ERR( "Couldn't initialize some/ip application" );
      return false;
   }

   m_app->register_state_handler( std::bind( &Server::on_state, this, std::placeholders::_1 ) );

   m_app->register_message_handler(
      SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, SAMPLE_METHOD_ID,
      std::bind( &Server::on_message, this, std::placeholders::_1 )
   );

   return true;
}

void Server::start( )
{
   MSG_VRB( );

   m_app->start( );
}

void Server::stop( )
{
   m_app->clear_all_handler( );
   m_app->stop_offer_service( SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID );
   m_app->stop( );
}

void Server::on_state( vsomeip::state_type_e state )
{
   MSG_DBG( "Application '%s' is %s", m_app->get_name( ).c_str( ), vsomeip::state_type_e::ST_REGISTERED == state ? "registered." : "deregistered." );
   m_is_registered = vsomeip::state_type_e::ST_REGISTERED == state;

   if( vsomeip::state_type_e::ST_REGISTERED == state )
   {
      m_app->offer_service( SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID );
   }
}

void Server::on_message( const std::shared_ptr< vsomeip::message >& message )
{
   response( message );
}

void Server::response( const std::shared_ptr< vsomeip::message >& _message )
{
   std::shared_ptr< vsomeip::message > message = vsomeip::runtime::get( )->create_response( _message );

   std::shared_ptr< vsomeip::payload > payload = vsomeip::runtime::get( )->create_payload( );
   std::vector< vsomeip::byte_t > payload_data{ 1 };
   payload->set_data( payload_data );
   message->set_payload( payload );

   m_app->send( message );
}
