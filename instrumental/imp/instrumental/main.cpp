#include "Elf64.hpp"



const bool DEBUG_INFO = false;
const std::size_t MAX_DEPTH = 4096; // sizeof( call_chain ) / sizeof( call_chain[0] )



char* call_chain[ MAX_DEPTH ];
std::size_t depth = 0;
FILE* log_file_handle = nullptr;
instrumental::Elf64* elf;



int getresuid( uid_t*, uid_t*, uid_t* ) __attribute__(( no_instrument_function ));

bool init( ) __attribute__(( no_instrument_function ));
bool init( )
{
   elf = new instrumental::Elf64;
   if( nullptr == elf )
      return false;
   if( false == elf->init( ) )
      return false;

   call_chain[0] = strdup( "root" );

   pid_t pid = getpid( );
   char log_file[ sizeof( elf->app.path ) + sizeof( pid_t ) + 4 /* for '.log' below */ ];
   snprintf( log_file, sizeof( log_file ), "%s.%d.log", elf->app.path, pid );
   if( nullptr == ( log_file_handle  = fopen( log_file, "a" ) ) )
   {
      printf( "open(%s) error: %s\n", log_file, strerror( errno ) );
      return false;
   }

   return true;
}



extern "C" {
   #if defined(__i386__)
      static __inline__ unsigned long long __rdtsc( void ) __attribute__(( no_instrument_function ));
      static __inline__ unsigned long long __rdtsc( void )
      {
         unsigned long long int x;
         __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
         return x;
      }
   #elif defined(__x86_64__)
      static __inline__ unsigned long long __rdtsc( void ) __attribute__(( no_instrument_function ));
      static __inline__ unsigned long long __rdtsc( void )
      {
         unsigned hi, lo;
         __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
         return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
      }
   #endif
}



extern "C" {
   void __cyg_profile_func_enter( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));
   void __cyg_profile_func_exit( void* this_fn, void* call_site ) __attribute__(( no_instrument_function ));

   void __cyg_profile_func_enter( void* this_fn, void* call_site )
   {
      // printf( "\nENTER: %p, <-- %p: %lld\n", this_fn, call_site, __rdtsc( ) );

      Dl_info info;

      void* p_extra_info = nullptr;
      if( 0 >= dladdr1( this_fn, &info, &p_extra_info, RTLD_DL_LINKMAP ) )
      {
         printf( "dladdr1 error\n" );
         return;
      }

      if( true == DEBUG_INFO )
      {
         printf( "this_fn: %p / call_site: %p\n", this_fn, call_site );

         printf( "info.dli_fname: %s\n", info.dli_fname );
         printf( "info.dli_fname: %p\n", info.dli_fbase );
         printf( "info.dli_fname: %s\n", info.dli_sname );
         printf( "info.dli_fname: %p\n", info.dli_saddr );

         if( nullptr != p_extra_info )
         {
            printf( "extra_info: %p\n", (link_map*)p_extra_info );
            printf( "extra_info.l_addr: %ld\n", ( (link_map*)p_extra_info )->l_addr );
            printf( "extra_info.l_name: %s\n", ( (link_map*)p_extra_info )->l_name );
            printf( "extra_info.l_ld: %p\n", ( (link_map*)p_extra_info )->l_ld );
            printf( "extra_info.l_next: %p\n", ( (link_map*)p_extra_info )->l_next );
            printf( "extra_info.l_prev: %p\n", ( (link_map*)p_extra_info )->l_prev );
         }         
      }

      if( nullptr == elf )
         init( );

      for( int i = 0; i < elf->nsyms; ++i )
      {
         auto& elf64_sym = elf->elf64_syms[ i ];

         if( ELF64_ST_TYPE( elf64_sym.st_info ) != STT_FUNC )
            continue;
         if( ( elf64_sym.st_value + (Elf64_Addr)info.dli_fbase ) != (Elf64_Addr)this_fn )
            continue;

         if( depth + 1 >= MAX_DEPTH )
         {
            printf( "MAX_DEPTH(%zu) overlimit\n", MAX_DEPTH );
            break;
         }

         call_chain[ depth + 1 ] = &(elf->strtab)[ elf64_sym.st_name ];
         break;
      }

      {
         uid_t uid, euid, ruid;
         getresuid( &ruid, &euid, &uid );
         fprintf( log_file_handle, "~ [%04d|%04d|%04d] ", ruid, euid, uid );

         for( std::size_t i = 0; i < depth; ++i )
            fprintf( log_file_handle, "  |" );
         fprintf( log_file_handle, "--" );

         fprintf( log_file_handle, "%s -> %s [depth=%zu];\n", call_chain[ depth ], call_chain[ depth + 1 ], depth );
      }

      ++depth;
   }

   void __cyg_profile_func_exit( void* this_fn, void* call_site )
   {
      // printf( "\nEXIT:  %p, --> %p: %lld\n", this_fn, call_site, __rdtsc( ) );

      --depth;
   }
}
