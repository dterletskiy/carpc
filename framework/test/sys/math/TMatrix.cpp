#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/tools/Performance.hpp"
#include "api/sys/math/TMatrix.hpp"
#include "test/sys/math/TMatrix.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:MATH:MATRIX"



namespace base::math::tmatrixd::test {

   bool run( )
   {
      base::math::TMatrix< std::size_t > matrix( base::math::tRow{ 4 }, base::math::tColumn{ 7 }, {
                                                                           10, 11, 12, 13, 14, 15, 16,
                                                                           20, 21, 22, 23, 24, 25, 26,
                                                                           30, 31, 32, 33, 34, 35, 36,
                                                                           40, 41, 42, 43, 44, 45, 46
                                                                     } );

      matrix.print( );
      matrix.minor( {
            std::make_pair( tRow{ 1 }, tColumn{ 2 } ),
            std::make_pair( tRow{ 3 }, tColumn{ 5 } )
         } ).print( );
      matrix.minor_a( {
            std::make_pair( tRow{ 1 }, tColumn{ 2 } ),
            std::make_pair( tRow{ 3 }, tColumn{ 5 } )
         } ).print( );


      return true;
   }



   bool run_backup( )
   {
      // SYS_TRC( "--------------- Creating matrix A ---------------" );
      // base::math::TMatrix< double > A( base::math::tRow{ 500 }, base::math::tColumn{ 4000 }, {
      //                                                                      10, 11, 12, 13,
      //                                                                      20, 21, 22, 23,
      //                                                                      30, 31, 32, 33
      //                                                                } );
      // SYS_TRC( "--------------- Creating matrix B ---------------" );
      // base::math::TMatrix< double > B( base::math::tRow{ 4000 }, base::math::tColumn{ 900 }, {
      //                                                                      100, 101, 102, 103, 104,
      //                                                                      200, 201, 202, 203, 204,
      //                                                                      300, 301, 302, 303, 304,
      //                                                                      400, 401, 402, 403, 404
      //                                                                } );

      // {
      //    SYS_ERR( "--------------- Multiplying matrixes A and B ---------------" );
      //    base::tools::Performance performance( "Myltiplication" );
      //    performance.start( "start" );
      //    A.multiply( B );
      //    performance.stop( "stop" );
      //    performance.info( );
      // }
      // {
      //    SYS_ERR( "--------------- Multiplying matrixes A and B ---------------" );
      //    base::tools::Performance performance( "Myltiplication" );
      //    performance.start( "start" );
      //    A.multiply_omp( B );
      //    performance.stop( "stop" );
      //    performance.info( );
      // }
      // SYS_ERR( "--------------- MARKER ---------------" );




      // return true;







#if 0
      SYS_ERR( "--------------- MARKER ---------------" );
      base::math::TMatrix< std::size_t > matrix_34( base::math::tRow{ 3 }, base::math::tColumn{ 4 }, {
                                                                           10, 11, 12, 13,
                                                                           20, 21, 22, 23,
                                                                           30, 31, 32, 33
                                                                     } );
      matrix_34.print( );

      base::math::TMatrix< std::size_t > matrix_34_new( base::math::tRow{ 3 }, base::math::tColumn{ 4 } );
      matrix_34_new.print( );
      matrix_34_new.apply( matrix_34, [ ]( const std::size_t& value ){ return value * 2; } );
      matrix_34_new.print( );

      matrix_34_new = matrix_34_new - matrix_34;
      matrix_34_new.print( );

      matrix_34_new = 10 * matrix_34;
      matrix_34_new.print( );

      SYS_ERR( "--------------- MARKER ---------------" );
      base::math::TMatrix< std::size_t > matrix_42( base::math::tRow{ 4 }, base::math::tColumn{ 2 }, {
                                                                           100, 101,
                                                                           200, 201,
                                                                           300, 301,
                                                                           400, 401
                                                                     } );
      matrix_42.print( );

      SYS_ERR( "--------------- MARKER ---------------" );
      srand( time( nullptr ) );
      base::math::TMatrix< float > matrix_57( base::math::tRow{ 5 }, base::math::tColumn{ 7 } );
      matrix_57.init( base::math::eCloneType::BothDiag, [ ]( ){ return static_cast< float >( rand( ) ) / RAND_MAX; } );
      matrix_57.print( );

      SYS_ERR( "--------------- MARKER ---------------" );
      base::math::multiply( matrix_34, matrix_42 ).print( );
      SYS_ERR( "--------------- MARKER ---------------" );
      base::math::transpose( matrix_42 ).print( );
      SYS_ERR( "--------------- MARKER ---------------" );
      matrix_42.T( ).print( );
      SYS_ERR( "--------------- MARKER ---------------" );
#endif

      return true;
   }

}