// Framework
#include "api/sys/asm/cpu.hpp"
#include "api/sys/oswrappers/linux/kernel.hpp"
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




#if 0
extern "C" {
   void __cyg_profile_func_enter( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));
   void __cyg_profile_func_exit( void *this_fn, void *call_site ) __attribute__(( no_instrument_function ));

   void __cyg_profile_func_enter( void* this_fn, void* call_site )
   {
      printf( "ENTER: %p, <-- %p: %lld\n", this_fn, call_site, __rdtsc( ) );
   }
   void __cyg_profile_func_exit( void *this_fn, void *call_site )
   {
      printf( "EXIT:  %p, --> %p: %lld\n", this_fn, call_site, __rdtsc( ) );
   }
}
#endif



void boot( )
{
   memory::dump( );

   REGISTER_DSI_EVENT( ServiceDSI, PingEvent );
   REGISTER_DSI_EVENT( ServiceOnOff, OnOffEvent );
   base::EventRegistry::instance( )->dump( );

   base::ServiceInfoVector services = { { "OnOff_Service", { application::onoff::creator }, 5 } };

   base::ServiceProcessPtr p_process = base::ServiceProcess::instance( );
   if( p_process->start( services ) )
   {
      DBG_MSG( "Booting..." );
      sleep(1);
      p_process->boot( );
   }



   DBG_TRC( "Main: program exiting." );
   DBG_INF( "Main: program exiting." );
   DBG_MSG( "Main: program exiting." );
   DBG_WRN( "Main: program exiting." );
   DBG_ERR( "Main: program exiting." );

   memory::dump( );
}



enum class eType : size_t { A, B, C, Undefined };
class Base
{
public:
   Base( const eType type ) : m_type( type ) { }
   virtual ~Base( ) = default;

   eType type( ) const { return m_type; }

private:
   eType m_type = eType::Undefined;
};

class A : public Base
{
public:
   A( ) : Base( eType::A ) { }
   ~A( ) override = default;

   void execute( )
   {
      // DBG_MSG( "class A" );
      ++m_count;
   }

private:
   size_t m_count = 0;
};

long long int test_no_cast( A* pA, const size_t count )
{
   long long int start = __rdtsc( );
   for( size_t i = 0; i < count; ++i )
   {
      pA->execute( );
   }
   long long int finish = __rdtsc( );
   long long int delta = finish - start;
   DBG_MSG( "%lld", delta );
   return delta;
}

long long int test_static_cast( Base* pBase, const size_t count )
{
   long long int start = __rdtsc( );
   for( size_t i = 0; i < count; ++i )
   {
      static_cast< A* >( pBase )->execute( );
   }
   long long int finish = __rdtsc( );
   long long int delta = finish - start;
   DBG_MSG( "%lld", delta );
   return delta;
}

long long int test_dynamic_cast( Base* pBase, const size_t count )
{
   long long int start = __rdtsc( );
   for( size_t i = 0; i < count; ++i )
   {
      dynamic_cast< A* >( pBase )->execute( );
   }
   long long int finish = __rdtsc( );
   long long int delta = finish - start;
   DBG_MSG( "%lld", delta );
   return delta;
}

struct Result
{
   float _no_cast = 0;
   float _static_cast = 0;
   float _dynamic_cast = 0;
};

Result test( const size_t count )
{
   DBG_MSG( "count = %zu", count );
   Base* pA = new A;
   float _no_cast = static_cast< float >( test_no_cast( static_cast< A* >( pA ), count ) );
   float _static_cast = static_cast< float >( test_static_cast( pA, count ) );
   float _dynamic_cast = static_cast< float >( test_dynamic_cast( pA, count ) );
   return { _no_cast / _no_cast, _static_cast / _no_cast, _dynamic_cast / _no_cast };
}

int main( int argc, char *argv[] )
{
   DBG_MSG( "argc = %d", argc );
   DBG_MSG( "SIGRTMIN = %d / SIGRTMAX = %d", SIGRTMIN, SIGRTMAX );



   std::vector< size_t > test_counts = { 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
   std::vector< Result > results;
   for( size_t count : test_counts )
   {
      results.push_back( test( count ) );
   }

   for( Result result : results )
   {
      DBG_MSG( "%f / %f / %f", result._no_cast, result._static_cast, result._dynamic_cast );
   }





   // boot( );
   return 0;
}
