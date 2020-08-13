#include <stdio.h>
#include <time.h>

#include <tbb/tbb.h>
#include <tbb/task_scheduler_init.h>

#include "api/sys/helpers/macros/strings.hpp"
#include "test/other/parallel/tbb/tbb.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:PARALLEL:TBB"



struct hello : public ::tbb::task
{
   task* execute ( )
   {
      DBG_WRN( "Hello, world!" );
      return nullptr;
   }
};

template< typename TYPE >
void print_range( ::tbb::blocked_range< TYPE >& range )
{
   for( TYPE index = range.begin( ); index != range.end( ); ++index )
      std::cout << index << " ";
   std::cout << std::endl << std::endl;
}



namespace other::parallel::tbb::test {

   bool run( )
   {
      SYS_ERR( "--------------- MARKER ---------------" );



      ::tbb::task_scheduler_init tsi( 4 );



      ::tbb::blocked_range< size_t > range_a( 0, 100 );
      print_range( range_a );

      ::tbb::blocked_range< size_t > range_b( range_a, ::tbb::split( ) );
      print_range( range_a );
      print_range( range_b );

      ::tbb::blocked_range< size_t > range_c( range_b, ::tbb::split( ) );
      print_range( range_a );
      print_range( range_b );
      print_range( range_c );



      hello& t = *new( ::tbb::task::allocate_root( ) ) hello;
      ::tbb::task::spawn_root_and_wait( t );

      auto function = [ ]( size_t index ) -> void
      {
         DBG_WRN( "index: %zu", index );
      };
      ::tbb::parallel_for( 0, 1000, function );







      return true;
   }

}
