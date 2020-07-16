// Framework
#include "api/sys/service/ServiceProcess.hpp"
#include "api/sys/tools/Tools.hpp"
// Application
#include "imp/app/components/OnOff/Component.hpp"
#include "imp/app/components/Driver/Component.hpp"
#include "imp/app/components/Master/Component.hpp"
#include "imp/app/components/Slave/Component.hpp"

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



#ifdef USE_INSTRUMENTAL
   #include "api/sys/asm/cpu.hpp"
   extern "C" {
      void __cyg_profile_func_enter( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));
      void __cyg_profile_func_exit( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));

      void __cyg_profile_func_enter( void* this_fn, void* call_site )
      {
         printf( "ENTER: %p, <-- %p: %lld\n", this_fn, call_site, __rdtsc( ) );
      }
      void __cyg_profile_func_exit( void* this_fn, void* call_site )
      {
         printf( "EXIT:  %p, --> %p: %lld\n", this_fn, call_site, __rdtsc( ) );
      }
   }
#endif



const bool test( int argc, char** argv );

void boot( int argc, char** argv )
{
   memory::dump( );
   DBG_MSG( "SIGRTMIN = %d / SIGRTMAX = %d", SIGRTMIN, SIGRTMAX );

   base::tools::cmd::init( argc, argv );

   REGISTER_EVENT( application::events::AppEvent );
   REGISTER_EVENT( api::onoff::ipc::OnOffEvent );
   DUMP_IPC_EVENTS;

   base::ServiceThread::Info::tVector services =
   {
        { "OnOff_Service", { application::components::onoff::Component::creator }, 5 }
      , { "Driver_Service", { application::components::driver::Component::creator }, 10 }
      , { "Device_Service", { application::components::master::Component::creator, application::components::slave::Component::creator }, 10 }
   };

   base::ServiceProcess::tSptr p_process = base::ServiceProcess::instance( );
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
}



#if OS == LINUX

   int main( int argc, char* argv[ ] )
   {
      DBG_MSG( "argc = %d", argc );

      if( test( argc, argv ) )
         boot( argc, argv );

      return 0;
   }

#elif OS == ANDROID

   #include <jni.h>
   #include "api/sys/oswrappers/Thread.hpp"

   base::os::Thread boot_thread __attribute__ (( section ("THREAD"), init_priority (101) )) = { boot };

   void __constructor__( ) __attribute__(( constructor(102) ));
   void __destructor__( ) __attribute__(( destructor(102) ));

   void __constructor__( ) { DBG_INF( "library loaded" ); }
   void __destructor__( ) { DBG_INF( "library unloaded" ); }

   extern "C" JNIEXPORT jstring JNICALL
   Java_com_tda_framework_MainActivity_jniStartFramework( JNIEnv* env, jobject /* this */ )
   {
      DBG_TRC( "JNI" );
      boot_thread.run( );

      return env->NewStringUTF( "@TDA: Hello from C++" );
   }

#endif





#include "api/sys/helpers/macros/strings.hpp"

#include <stdio.h>
#include <omp.h>
#include <time.h>

#include <tbb/tbb.h>
#include <tbb/task_scheduler_init.h>






struct hello : public tbb::task
{
   task* execute ( )
   {
      DBG_WRN( "Hello, world!" );
      return nullptr;
   }
};

template< typename TYPE >
void print_range( tbb::blocked_range< TYPE >& range )
{
   for( TYPE index = range.begin( ); index != range.end( ); ++index )
      std::cout << index << " ";
   std::cout << std::endl << std::endl;
}





double VectorsMultiplication( const double *a, const double *b, size_t size )
{
   double result = 0.0;
   for( size_t i = 0; i < size; ++i )
      result += a[i] * b[i];
   return result;
}

class VectorsMultiplicator
{
   private:
      const double* matrix;
      const double* vector;
      double* const result;
      size_t const columns;

   public:
      VectorsMultiplicator( double* _matrix, double* _vector, double* _result, size_t _columns )
         : matrix( _matrix )
         , vector( _vector )
         , result( _result )
         , columns( _columns )
      { }

      void operator( )( const tbb::blocked_range< size_t >& range ) const
      {
         // DBG_MSG( );
         for( size_t i = range.begin( ); i != range.end( ); ++i )
            result[ i ] = VectorsMultiplication( &( matrix[ i * columns ] ), vector, columns );
      }
};



template< typename T >
T* allocate( const size_t rows, const size_t columns )
{
   if( 0 == rows && 0 == columns )
      return nullptr;

   if( 0 == rows )
      return (T*)malloc( sizeof( T* ) * columns ) ;

   if( 0 == columns )
      return (T*)malloc( sizeof( T* ) * rows );

   return (T*)malloc( sizeof( T* ) * rows * columns );
}

template< typename T >
void deallocate( T* vector )
{
   if( nullptr != vector )
      free( vector );
}

const size_t s_value = 12;
template< typename T >
void init( T* vector, const size_t rows, const size_t columns )
{
   if( 0 == rows && 0 == columns )
      return;

   if( 0 == rows )
   {
      for( size_t j = 0; j < columns; ++j )
         vector[ j ] = s_value;//j;
      return;
   }

   if( 0 == columns )
   {
      for( size_t j = 0; j < rows; ++j )
         vector[ j ] = s_value;//j;
      return;
   }

   for( size_t i = 0; i < rows; ++i )
   {
      for( size_t j = 0; j < columns; ++j )
         vector[ i * columns + j ] = s_value;//i + j;
   }
}

const bool sNoDebug = true;

template< typename T >
void print( T* vector, const size_t rows, const size_t columns )
{
   if( sNoDebug )
      return;

   DBG_MSG( "--------------- DUMP BEGIN ---------------" );
   DBG_MSG( "rows: %zu", rows );
   DBG_MSG( "columns: %zu", columns );

   if( 0 == rows && 0 == columns )
   {
      return;
   }
   if( 0 == rows )
   {
      for( size_t j = 0; j < columns; ++j )
         std::cout << vector[ j ] << " ";
      std::cout << std::endl;
      return;
   }
   if( 0 == columns )
   {
      for( size_t j = 0; j < rows; ++j )
         std::cout << vector[ j ] << " ";
      std::cout << std::endl;
      return;
   }
   else
   {
      for( size_t i = 0; i < rows; ++i )
      {
         for( size_t j = 0; j < columns; ++j )
            std::cout << vector[ i * columns + j ] << " ";
         std::cout << std::endl;
      }
   }
   std::cout << std::endl;

   DBG_MSG( "---------------- DUMP END ----------------" );
}

void matrix_vector_product_omp( double* a, double* b, double* c, int m, int n )
{
   #pragma omp parallel num_threads( 4 )
   {
      int nthreads = 4;//omp_get_num_threads( );
      int threadid = omp_get_thread_num( );
      // DBG_MSG( "Hello, multithreaded world: thread %d of %d", threadid, nthreads );

      int items_per_thread= m / nthreads;
      int lb= threadid* items_per_thread;
      int ub= ( threadid == nthreads - 1 ) ? ( m - 1 ) : ( lb + items_per_thread - 1 );
      for( int i= lb; i <= ub; i++ )
      {
         c[i] = 0.0;
         for( int j = 0; j < n; j++ )
            c[i] += a[i * n + j] * b[j];
      }
   }
}





const bool test( int argc, char** argv )
{
   // return true;
   SYS_ERR( "--------------- MARKER ---------------" );



   #if 0 // OMP

      auto matrix_vector_product = [ ]( double* a, double* b, double* c, size_t m, size_t n ) -> void
      {
         for( size_t i = 0; i < m; i++ )
         {
            c[ i ] = 0.0;
            for( size_t j = 0; j < n; ++j )
               c[ i ] += a[ i * n + j ] * b[ j ];
         }
      };

      auto run_serial = [ ]( const size_t m, const size_t n ) -> void
      {
         // Allocate memory for 2-d array a[m, n]
         double* a = (double*)malloc( sizeof( *a ) * m * n );
         double* b = (double*)malloc( sizeof( *b ) * n ) ;
         double* c = (double*)malloc( sizeof( *c ) * m );

         for( size_t i = 0; i < m; ++i )
         {
            for( size_t j = 0; j < n; ++j )
               a[ i * n + j ] = i + j;
         }

         for( size_t j = 0; j < n; ++j )
            b[ j ] = j;

         base::tools::Performance performance( "serial" );
         performance.start( );
         matrix_vector_product( a, b, c, m, n );
         performance.stop( );

         free( a );
         free( b );
         free( c );
      };

      const size_t m = 10000;
      const size_t n = 10000;

      DBG_MSG( "Matrix-vector product (c[m] = a[m, n] * b[n]; m = %zu, n = %zu)", m, n );
      DBG_MSG( "Memory used: %"PRIu64 " MiB", ((m * n + m + n) * sizeof(double)) >> 20 );

      run_serial( m, n );

   #endif



   #if 0 // TBB vs OMP

      tbb::task_scheduler_init tsi( 4 );





      const size_t rows = 30000;
      const size_t columns = 30000;
      const size_t grainsize = 25000;

      double* matrix = allocate< double >( rows, columns );
      double* vector = allocate< double >( 0, columns );
      double* result = allocate< double >( rows, 0 );

      init( matrix, rows, columns );
      init( vector, 0, columns );



      long int TBB = 0, OMP_1 = 0, OMP_2 = 0;
      const size_t count = 10;
      for( size_t iteration = 0; iteration < count; ++iteration )
      {
         DBG_WRN( "---------------------------------------------------- %zu ----------------------------------------------------", iteration );

         {
            SYS_ERR( "--------------- TBB ---------------" );

            print( matrix, rows, columns );
            print( vector, 0, columns );
            print( result, rows, 0 );

            base::tools::Performance performance( "TBB" );
            performance.start( "start" );
            tbb::parallel_for( tbb::blocked_range< size_t >( 0, rows, grainsize ), VectorsMultiplicator( matrix, vector, result, columns ) );
            performance.stop( "stop" );
            TBB += performance.info( );

            print( result, rows, 0 );
         }

         {
            SYS_ERR( "--------------- OMP ---------------" );

            print( matrix, rows, columns );
            print( vector, 0, columns );
            print( result, rows, 0 );

            base::tools::Performance performance( "OMP" );
            performance.start( "start" );
            #pragma omp parallel for schedule( dynamic, grainsize )
               for( size_t i = 0; i < rows; ++i )
                  result[i] = VectorsMultiplication( &( matrix[ i * columns ] ), vector, columns );
            performance.stop( "stop" );
            OMP_1 += performance.info( );

            print( result, rows, 0 );
         }

         {
            SYS_ERR( "--------------- OMP ---------------" );

            print( matrix, rows, columns );
            print( vector, 0, columns );
            print( result, rows, 0 );

            base::tools::Performance performance( "OMP" );
            performance.start( "start" );
            matrix_vector_product_omp( matrix, vector, result, rows, columns );
            performance.stop( "stop" );
            OMP_2 += performance.info( );

            print( result, rows, 0 );
         }

         DBG_WRN( "--------------------------------------------------------------------------------------------------------\n\n\n" );
      }

      deallocate( matrix );
      deallocate( vector );
      deallocate( result );

      DBG_TRC( "TBB: %ld / TBB / count: %ld", TBB, TBB / count );
      DBG_TRC( "OMP_1: %ld / OMP_1 / count: %ld", OMP_1, OMP_1 / count );
      DBG_TRC( "OMP_2: %ld / OMP_2 / count: %ld", OMP_2, OMP_2 / count );





      // tbb::blocked_range< size_t > range_a( 0, 100 );
      // print_range( range_a );

      // tbb::blocked_range< size_t > range_b( range_a, tbb::split( ) );
      // print_range( range_a );
      // print_range( range_b );

      // tbb::blocked_range< size_t > range_c( range_b, tbb::split( ) );
      // print_range( range_a );
      // print_range( range_b );
      // print_range( range_c );







      // hello& t = *new( tbb::task::allocate_root( ) ) hello;
      // tbb::task::spawn_root_and_wait( t );

      // auto function = [ ]( size_t index ) -> void
      // {
      //    DBG_WRN( "index: %zu", index );
      // };
      // tbb::parallel_for( 0, 1000, function );

   #endif



   return false;
}
