// Framework
#include "api/sys/common/Helpers.hpp"
#include "api/sys/oswrappers/linux/kernel.hpp"
#include "api/sys/oswrappers/Mutex.hpp"
#include "api/sys/oswrappers/Thread.hpp"
#include "api/sys/tools/Performance.hpp"
#include "api/sys/tools/Tools.hpp"
#include "api/sys/service/ServiceProcess.hpp"
// Application
#include "imp/app/components/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



namespace memory {
   #ifdef HOOK_MEMORY_ALLOCATOR
      extern void dump( );
      extern void set_track_size( const size_t );
   #else
      inline void dump( ) { }
      inline void set_track_size( const size_t ) { }
   #endif
}




int main( int argc, char *argv[] )
{

#if 0
   base::ByteBufferT buffer;
   bool result = false;

   std::string string = "DSI RPC";
   buffer.push( string );
   buffer.dump( );
   string = { };
   buffer.pop( string );
   buffer.dump( );
   base::print( string, true );
   buffer.push( string );
   buffer.dump( );
   string = { };

   std::vector< std::string > vector { "One", "Two", "Three", "Four", "Five" };
   // buffer.push( vector );
   // buffer.dump( );
   vector = { };
   result = buffer.pop( vector );
   buffer.dump( );
   DBG_INF( "result: %s", BOOL_TO_STRING( result ) );
   base::print( vector, true );

   result = buffer.pop( string );
   buffer.dump( );
   DBG_INF( "result: %s", BOOL_TO_STRING( result ) );
   base::print( string, true );

   std::map< size_t, std::vector< std::string > > map {
      { 1, { "1_One", "1_Two", "1_Three", "1_Four", "1_Five" } },
      { 2, { "2_One", "2_Two", "2_Three", "2_Four", "2_Five" } },
      { 3, { "3_One", "3_Two", "3_Three", "3_Four", "3_Five" } }
   };
   base::print( map, true );
   buffer.push( map );
   buffer.dump( );
   map = { };

   vector = { };
   result = buffer.pop( vector );
   buffer.dump( );
   DBG_INF( "result: %s", BOOL_TO_STRING( result ) );
   base::print( vector, true );

   result = buffer.pop( map );
   DBG_INF( "result: %s", BOOL_TO_STRING( result ) );
   base::print( map, true );
   DBG_MSG( "-----------------------------------------------------------------------" );
#endif

#if 1
   memory::dump( );

   REGISTER_DSI_EVENT( xDsiPingEvent, DsiService );
   base::EventRegistry::instance( )->dump( );

   base::ServiceInfoVector services = { { "OnOff_Service", { application::onoff::creator }, 5 } };

   base::ServiceProcessPtr p_process = base::ServiceProcess::instance( );
   if( p_process->start( services ) )
   {
      DBG_MSG( "Booting..." );
      p_process->boot( );
   }



   DBG_TRC( "Main: program exiting." );
   DBG_INF( "Main: program exiting." );
   DBG_MSG( "Main: program exiting." );
   DBG_WRN( "Main: program exiting." );
   DBG_ERR( "Main: program exiting." );

   memory::dump( );
#endif



   return 0;
}
