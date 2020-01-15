// Framework
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/tools/Tools.hpp"
// Application
#include "imp/app/components/OnOff.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



namespace memory {
   #ifdef HOOK_MEMORY_ALLOCATOR
      extern void dump( );
   #else
      inline void dump( ) { }
   #endif
}



#if 0
#include "oswrappers/Thread.hpp"

void thread_loop( int index )
{
   int count = 0;
   while( count < 10000 )
   {
      DBG_TRC( "Thread ID: %#x / index: %d", pthread_self( ), index );
      ++count;
   }
}

base::os::Thread thread __attribute__ (( section ("THREAD"), init_priority (101) )) = { thread_loop, 45 };

void __constructor__( ) __attribute__(( constructor(102) ));
void __destructor__( ) __attribute__(( destructor(102) ));

void __constructor__( )
{
   DBG_INF( );
   // thread.run( );
}

void __destructor__( )
{
   DBG_INF( );
   // thread.join( );
}
#endif





#include "api/sys/common/ByteBuffer.hpp"

template< typename TYPE >
void print( const TYPE& value )
{
   std::cout << value;
}

template< typename TYPE >
void print( const std::list< TYPE >& list )
{
   std::cout << "{ " ;
   for ( const auto& value : list )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";
}

template< typename TYPE >
void print( const std::vector< TYPE >& vector )
{
   std::cout << "{ " ;
   for ( const auto& value : vector )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";
}

template< typename TYPE >
void print( const std::set< TYPE >& set )
{
   std::cout << "{ " ;
   for ( const auto& value : set )
   {
      print( value );
      std::cout << " ";
   }
   std::cout << "}";
}

template< typename TYPE_KEY, typename TYPE_VALUE >
void print( const std::pair< TYPE_KEY, TYPE_VALUE >& pair )
{
   std::cout << "{ " ;
   print( pair.first );
   std::cout << " => ";
   print( pair.second );
   std::cout << " }";
}

template< typename TYPE_KEY, typename TYPE_VALUE >
void print( const std::map< TYPE_KEY, TYPE_VALUE >& map )
{
   std::cout << "{ " ;
   for ( const auto& pair : map )
   {
      print( pair );
      std::cout << " ";
   }
   std::cout << " }";
}


int main( int argc, char *argv[] )
{
   {
      std::cout << "std::vector< int >:" << std::endl;
      std::vector< int > vector = { 111, 222, 333, 444, 555 };
      base::ByteBuffer buffer;
      buffer.push( vector );
      buffer.dump( );

      std::vector< int > vector_new = { 0, 0, 0, 0, 0 };
      buffer.pop( vector_new );
      print( vector_new );
      std::cout << std::endl << std::endl;
   }
   {
      std::cout << "std::list< int >:" << std::endl;
      std::list< int > list = { 111, 222, 333, 444, 555 };
      base::ByteBuffer buffer;
      buffer.push( list );
      buffer.dump( );

      std::list< int > list_new = { 0, 0, 0, 0, 0 };
      buffer.pop( list_new );
      print( list_new );
      std::cout << std::endl << std::endl;
   }
   {
      std::cout << "std::set< int >:" << std::endl;
      std::set< int > set = { 111, 222, 333, 444, 555 };
      base::ByteBuffer buffer;
      buffer.push( set );
      buffer.dump( );

      std::set< int > set_new = { 0, 1, 2, 3, 4 };
      buffer.pop( set_new );
      print( set_new );
      std::cout << std::endl << std::endl;
   }
   {
      std::cout << "std::pair< int, int >:" << std::endl;
      std::pair< int, int > pair = { 123,456 };
      base::ByteBuffer buffer;
      buffer.push( pair );
      buffer.dump( );

      std::pair< int, int > pair_new = { 0, 0 };
      buffer.pop( pair_new );
      print( pair_new );
      std::cout << std::endl << std::endl;
   }
   {
      std::cout << "std::map< int, int >:" << std::endl;
      std::map< int, int > map = { {111,222}, {333,444}, {555,666} };
      base::ByteBuffer buffer;
      buffer.push( map );
      buffer.dump( );

      std::map< int, int > map_new = { {0,0}, {1,1}, {2,2} };
      buffer.pop( map_new );
      print( map_new );
      std::cout << std::endl << std::endl;
   }
   {
      std::cout << "std::map< int, std::string >:" << std::endl;
      std::map< int, std::string > map = { {111,"222"}, {333,"444"}, {555,"666"} };
      base::ByteBuffer buffer;
      buffer.push( map );
      buffer.dump( );

      std::map< int, std::string > map_new = { {0,"0"}, {1,"1"}, {2,"2"} };
      buffer.pop( map_new );
      print( map_new );
      std::cout << std::endl << std::endl;
   }
   {
      std::cout << "std::map< int, std::vector< std::string > >:" << std::endl;
      std::map< int, std::vector< std::string > > map =
      {
         { 111, { "One", "Two", "Three" } },
         { 222, { "Four", "Five", "Six" } }
      };
      base::ByteBuffer buffer;
      buffer.push( map );
      buffer.dump( );

      std::map< int, std::vector< std::string > > map_new;
      buffer.pop( map_new );
      print( map_new );
      std::cout << std::endl << std::endl;
   }
   







#if 0
   memory::dump( );

   REGISTER_DSI_EVENT( DsiPingEvent, DsiService );
   REGISTER_DSI_EVENT( xDsiPingEvent, xDsiService );
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
