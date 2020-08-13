#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/math/TMatrixS.hpp"
#include "test/sys/math/TMatrixS.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:MATH:TMATRIX"



namespace base::math::tmatrixs::test {

   bool run( )
   {
      #if __cplusplus > 201703L // C++2a

         {
            SYS_ERR( "--------------- math::c2a ---------------" );

            base::math::tTMatrixS< size_t, base::math::tRow{ 3 }, base::math::tColumn{ 4 } > matrix_34( "matrix_34", {
                                                                        10, 11, 12, 13,
                                                                        20, 21, 22, 23,
                                                                        30, 31, 32, 33
                                                } );
            matrix_34.print( );
            base::math::tTMatrixS< size_t, base::math::tRow{ 4 }, base::math::tColumn{ 5 } > matrix_45( "matrix_45", {
                                                                        10, 11, 12, 13, 14,
                                                                        20, 21, 22, 23, 24,
                                                                        30, 31, 32, 33, 34,
                                                                        40, 41, 42, 43, 44,
                                                } );
            matrix_45.print( );
            base::math::tTVectorSCV< size_t, base::math::tColumn{ 4 } > vector_14( "vector_14", { 100, 200, 300, 400 } );
            vector_14.print( );
            base::math::tTVectorSCTV< size_t, base::math::tRow{ 4 } > vector_41( "vector_41", {
                                                                     100,
                                                                     200,
                                                                     300,
                                                                     400
                                                } );
            vector_41.print( );

            base::math::multiply( matrix_34, matrix_45 ).print( );
            DBG_MSG( "value = %zu", matrix_34.get( base::math::tRow{ 1 }, base::math::tColumn{ 2 } ) );
         }

      #else

         {
            SYS_ERR( "--------------- math ---------------" );

            base::math::tTMatrixS< size_t, 3, 4 > matrix_34( "matrix_34", {
                                                                        10, 11, 12, 13,
                                                                        20, 21, 22, 23,
                                                                        30, 31, 32, 33
                                                } );

            matrix_34.print( );
            base::math::tTMatrixS< size_t, 4, 5 > matrix_45( "matrix_45", {
                                                                        10, 11, 12, 13, 14,
                                                                        20, 21, 22, 23, 24,
                                                                        30, 31, 32, 33, 34,
                                                                        40, 41, 42, 43, 44,

                                                } );
            matrix_45.print( );
            base::math::tTVectorSCV< size_t, 4 > vector_14( "vector_14", { 100, 200, 300, 400 } );
            vector_14.print( );
            base::math::tTVectorSCTV< size_t, 4 > vector_41( "vector_41", {
                                                                     100,
                                                                     200,
                                                                     300,
                                                                     400
                                                } );
            vector_41.print( );

            base::math::multiply( matrix_34, matrix_45 ).print( );
            base::math::multiply( matrix_34, vector_41 ).print( );
            base::math::multiply( vector_14, vector_41 ).print( );
            base::math::multiply( vector_41, vector_14 ).print( );

            DBG_MSG( "value = %zu", matrix_34.get( base::math::tRow{ 1 }, base::math::tColumn{ 2 } ) );
            matrix_34.get( base::math::tRow{ 2 } ).print( );
            matrix_34.get( base::math::tColumn{ 2 } ).print( );
            const size_t result = matrix_34;
            DBG_MSG( "operator( ) = %zu", result );
         }

      #endif

      return true;
   }

}