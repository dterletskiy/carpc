#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/comm/event/Event.hpp"
#include "api/sys/dsi/Types.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:DSI_TYPES"



namespace {

   struct AppEventData
   {
      AppEventData( ) { }
      AppEventData( const std::string& _message ) : message( _message ) { }
      AppEventData( const AppEventData& data ) : message( data.message ) { }
      AppEventData( base::dsi::tByteStream& stream ) { from_stream( stream ); }
      ~AppEventData( ) { }

      bool to_stream( base::dsi::tByteStream& stream ) const { return stream.push( message ); }
      bool from_stream( base::dsi::tByteStream& stream ) { return stream.pop( message ); }

      std::string message = "";
   };

   enum class eAppEventID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eAppEventID id )
   {
      switch( id )
      {
         case eAppEventID::BOOT:       return "BOOT";
         case eAppEventID::SHUTDOWN:   return "SHUTDOWN";
         case eAppEventID::PING:       return "PING";
         case eAppEventID::UNDEFINED:  return "UNDEFINED";
         default:                      return "UNDEFINED";
      }
   }

   using tAppEventSignature = base::async::TSignatureID< eAppEventID >;

   DEFINE_EVENT( AppEvent, AppEventData, tAppEventSignature );

} // namespace



namespace base::dsi::types::test {

   bool run( )
   {
      using tEvent = AppEvent::Event;
      REGISTER_EVENT( AppEvent );

      base::dsi::tByteStream stream;

      {
         base::dsi::Packet packet;
         DBG_WRN( "adding package" );
         packet.add_package( base::dsi::eCommand::RegisterServer, std::string( "PackageOne" ), (size_t)0xAAAAAAAA );
         DBG_WRN( "adding package" );
         packet.add_package( base::dsi::eCommand::UnregisterServer, std::string( "PackageTwo" ), (size_t)0xBBBBBBBB );
         DBG_WRN( "adding package" );
         packet.add_package( base::dsi::eCommand::RegisterClient, std::string( "PackageThree" ), (size_t)0xCCCCCCCC );
         DBG_WRN( "adding package" );
         packet.add_package( base::dsi::eCommand::UnregisterClient, std::string( "PackageFour" ), (size_t)0xDDDDDDDD );
         DBG_WRN( "adding package" );
         auto event = tEvent::create( { eAppEventID::BOOT }, { "booting system" }, base::async::eCommType::ITC );
         packet.add_package( base::dsi::eCommand::BroadcastEvent, *event, (size_t)0xEEEEEEEE );
         DBG_WRN( "serrialize packet" );
         stream.push( packet );
         stream.dump( );
      }

      {
         base::dsi::Packet packet;
         DBG_WRN( "deserrialize packet" );
         stream.pop( packet );


         for( auto& pkg : packet.packages( ) )
         {
            std::string string( "NoNe" );
            size_t value = 0;
            bool result = false;

            DBG_WRN( "extracting package" );
            if( base::dsi::eCommand::BroadcastEvent == pkg.command( ) )
            {
               base::async::IEvent::tSptr p_event = base::async::IEvent::deserialize( pkg.data( ) );
               if( nullptr == p_event )
               {
                  DBG_ERR( "event null pointer" );
               }
               if( nullptr == std::static_pointer_cast< tEvent >( p_event )->data( ) )
               {
                  DBG_ERR( "event data null pointer" );
               }
               else
               {
                  DBG_MSG( "event: %s", std::static_pointer_cast< tEvent >( p_event )->data( )->message.c_str( ) );
               }
            }
            else
            {
               result = pkg.data( string, value );
               DBG_MSG( "command: %s / result: %s / string: %s / value: %zX", base::dsi::c_str( pkg.command( ) ), BOOL_TO_STRING( result ), string.c_str( ), value );
            }
         }
      }

      return true;
   }

}
