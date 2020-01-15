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





template< typename T >
void doSomething( T arg )
{
   std::cout << __PRETTY_FUNCTION__ << ": -> " << arg << std::endl;
}

template< typename... Args >
void doSomethingForAll( Args const&... args )
{
   (void)std::initializer_list<int>{ ((void)doSomething(args),0)... };
}



class Base
{
public:
   Base( const int _value ) : m_value( _value ) { DBG_INF( ); }
   virtual ~Base( ) { DBG_INF( ); }

   void ping( )
   {
      DBG_INF( "%d", m_value );
      size_t* p_this = reinterpret_cast< size_t* >( this );
      if( 0 != *p_this )
      {
         request( );
      }
      else
      {
         DBG_ERR( "this = %p / vptr = %#lx", p_this, *p_this );
      }
   }
   virtual void request( ) = 0;

private:
   int m_value = 0;
};

class Derived : public Base
{
public:
   Derived( const int _value ) : Base( _value ), m_value( 2*_value ) { DBG_INF( ); }
   ~Derived( ) override { DBG_INF( ); }

   void request( ) override { DBG_INF( "%d", m_value ); }

private:
   int m_value = 0;
};



int main( int argc, char *argv[] )
{
   // doSomethingForAll( "Hello", 123, 123.456 );

   Base* p = new Derived( 100 );
   p->ping( );
   delete p;
   p->ping( );


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

#if 0
   memory::dump( );

   REGISTER_DSI_EVENT( ServiceDSI, PingEventDSI );
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
