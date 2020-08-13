#include <omp.h>

#include "api/sys/helpers/macros/strings.hpp"
#include "test/other/parallel/omp/omp.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:PARALLEL:TBB"



namespace other::parallel::omp::test {

   bool run( )
   {
      SYS_ERR( "--------------- MARKER ---------------" );



      DBG_MSG( "Main thread" );
      srand( time( nullptr ) );

      #pragma omp parallel num_threads( 8 )
      {
         DBG_MSG( "thread: %d(%d) / -->", omp_get_thread_num( ), omp_get_num_threads( ) );

         #pragma omp sections
         {
            // DBG_MSG( "sections / thread: %d(%d) / -->", omp_get_thread_num( ), omp_get_num_threads( ) );
            // DBG_MSG( "sections -->" );

            #pragma omp section
            {
               size_t time = 5;//rand( ) % 3 + 1;
               DBG_MSG( "section: 0 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
               sleep( time );
               DBG_MSG( "section: 0 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            }
            #pragma omp section
            {
               size_t time = 4;//rand( ) % 3 + 1;
               DBG_MSG( "section: 1 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
               sleep( time );
               DBG_MSG( "section: 1 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            }
            #pragma omp section
            {
               size_t time = 3;//rand( ) % 3 + 1;
               DBG_MSG( "section: 2 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
               sleep( time );
               DBG_MSG( "section: 2 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            }
            #pragma omp section
            {
               size_t time = 2;//rand( ) % 3 + 1;
               DBG_MSG( "section: 3 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
               sleep( time );
               DBG_MSG( "section: 3 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            }
            #pragma omp section
            {
               size_t time = 1;//rand( ) % 3 + 1;
               DBG_MSG( "section: 4 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
               sleep( time );
               DBG_MSG( "section: 4 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            }

            // DBG_MSG( "sections / thread: %d(%d) / <--", omp_get_thread_num( ), omp_get_num_threads( ) );
            // DBG_MSG( "sections <--" );
         }

         DBG_MSG( "thread: %d(%d) / <--", omp_get_thread_num( ), omp_get_num_threads( ) );
      }

      SYS_ERR( "--------------- MARKER ---------------" );

      #pragma omp parallel num_threads( 8 )
      {
         DBG_MSG( "thread: %d(%d) / -->", omp_get_thread_num( ), omp_get_num_threads( ) );

         #pragma omp single
         {
            size_t time = 5;//rand( ) % 3 + 1;
            DBG_MSG( "single: 0 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            sleep( time );
            DBG_MSG( "single: 0 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
         }
         #pragma omp single
         {
            size_t time = 4;//rand( ) % 3 + 1;
            DBG_MSG( "single: 1 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            sleep( time );
            DBG_MSG( "single: 1 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
         }
         #pragma omp single
         {
            size_t time = 3;//rand( ) % 3 + 1;
            DBG_MSG( "single: 2 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            sleep( time );
            DBG_MSG( "single: 2 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
         }
         #pragma omp single
         {
            size_t time = 2;//rand( ) % 3 + 1;
            DBG_MSG( "single: 3 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            sleep( time );
            DBG_MSG( "single: 3 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
         }
         #pragma omp single
         {
            size_t time = 1;//rand( ) % 3 + 1;
            DBG_MSG( "single: 4 / thread: %d(%d) / --> sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
            sleep( time );
            DBG_MSG( "single: 4 / thread: %d(%d) / <-- sleep: %zu", omp_get_thread_num( ), omp_get_num_threads( ), time );
         }

         DBG_MSG( "thread: %d(%d) / <--", omp_get_thread_num( ), omp_get_num_threads( ) );
      }

      // #pragma omp parallel num_threads( 8 )
      // {
      //    int threads = omp_get_num_threads( );
      //    int thread_id = omp_get_thread_num( );
      //    int threads_max = omp_get_max_threads( );
      //    int procs = omp_get_num_procs( );
      //    int is_parallel = omp_in_parallel( );
      //    DBG_WRN( "thread: %d(%d) / cpus: %d", thread_id, threads, procs );

      //    #pragma omp task
      //    {
      //       DBG_WRN( "Logic thread: %d of %d", omp_get_thread_num( ), omp_get_num_threads( ) );
      //    }
      //    #pragma omp taskwait

      //    #pragma omp barrier

      //    #pragma omp for schedule( static, 5 )
      //       for( size_t i = 0; i < 40; ++i )
      //       {
      //          int thread_id = omp_get_thread_num( );
      //          int threads = omp_get_num_threads( );
      //          size_t time = rand( ) % 3 + 1;
      //          DBG_MSG( "thread: %d(%d) / iteration: %zu / start sleep: %zu", thread_id, threads, i, time );
      //          sleep( time );
      //          DBG_MSG( "thread: %d(%d) / iteration: %zu / stop sleep: %zu", thread_id, threads, i, time );
      //       }
      // }







      return true;
   }

}
