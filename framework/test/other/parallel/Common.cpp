#include <stdio.h>
#include <omp.h>
#include <time.h>

#include <tbb/tbb.h>
#include <tbb/task_scheduler_init.h>

#include "api/sys/tools/Performance.hpp"
#include "api/sys/helpers/macros/strings.hpp"
#include "test/other/parallel/Common.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:PARALLEL:COMMON"



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

      void operator( )( const ::tbb::blocked_range< size_t >& range ) const
      {
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



namespace other::parallel::common::test {

   bool run( )
   {
      SYS_ERR( "--------------- MARKER ---------------" );



      ::tbb::task_scheduler_init tsi( 4 );

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
            ::tbb::parallel_for( ::tbb::blocked_range< size_t >( 0, rows, grainsize ), VectorsMultiplicator( matrix, vector, result, columns ) );
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



      return true;
   }

}