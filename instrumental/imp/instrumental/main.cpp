#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <link.h>
#include <elf.h>

#include "Application.hpp"



const bool DEBUG_INFO = false;
const std::size_t MAX_DEPTH = 4096; // sizeof( call_chain ) / sizeof( call_chain[0] )



Elf64_Sym* elf64_syms = nullptr;
char* strtab = nullptr;
int nsyms = 0;
char* call_chain[ MAX_DEPTH ];
std::size_t depth = 0;



int getresuid( uid_t *, uid_t *, uid_t * ) __attribute__(( no_instrument_function ));
int fill_tab( instrumental::Application& app, const Elf64_Shdr* shdr, char** result ) __attribute__(( no_instrument_function ));
bool init_instrumental( ) __attribute__(( no_instrument_function ));



int fill_tab( instrumental::Application& app, const Elf64_Shdr* shdr, char** result )
{
   off_t offset = app.lseek( 0, SEEK_CUR);
   if( (off_t)-1 == offset )
      return -1;

   if( (off_t)-1 == app.lseek( shdr->sh_offset, SEEK_SET ) )
      return -1;

   free( *result );
   *result = (char*)malloc( shdr->sh_size );
   int return_result = app.read( *result, shdr->sh_size );

   if( offset != app.lseek( offset, SEEK_SET ) )
      return -1;

   return return_result;
}



bool init_instrumental( )
{
   // const char* const log_file_base = "/home/scorpion/Desktop/instrumental.log";
   // char log_file[256];
   // FILE* log_file_handle = nullptr;
   // snprintf( log_file, sizeof( log_file ), "%s.%d", log_file_base, getpid( ) );
   // if( nullptr == ( log_file_handle  = fopen( log_file, "a" ) ) )
   // {
   //    printf( "open(%s) error: %s\n", log_file, strerror( errno ) );
   //    return false;
   // }



   Elf64_Ehdr elf64_ehdr;
   Elf64_Shdr elf64_shdr;

   instrumental::Application app;
   // app.print( );
   app.open( O_RDONLY );

   app.read( &elf64_ehdr, sizeof(elf64_ehdr), true );
   app.lseek( elf64_ehdr.e_shoff + elf64_ehdr.e_shstrndx * elf64_ehdr.e_shentsize, SEEK_SET );

   app.read( &elf64_shdr, sizeof(elf64_shdr), true );
   app.lseek( elf64_shdr.sh_offset, SEEK_SET );

   char* shstrtab = ( char* )malloc( elf64_shdr.sh_size );
   app.read( shstrtab, elf64_shdr.sh_size, true );
   app.lseek( elf64_ehdr.e_shoff, SEEK_SET );

   bool done = false;
   while( !done )
   {
      ssize_t r = app.read( &elf64_shdr, sizeof( elf64_shdr ) );
      if( 0 == strcmp( &shstrtab[ elf64_shdr.sh_name ], ".strtab" ) )
      { 
         fill_tab( app, &elf64_shdr, &strtab );
      }
      if( elf64_shdr.sh_type == SHT_SYMTAB )
      {
         r = fill_tab( app, &elf64_shdr, (char**)&elf64_syms );
         nsyms = r / sizeof( Elf64_Sym );
      }
      done = elf64_syms != nullptr && strtab != nullptr;
   }

   app.close( );

   call_chain[0] = strdup( "root" );

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

      Elf64_Addr global_offset = (Elf64_Addr)info.dli_fbase;

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
            printf( "extra_info.l_addr: %p\n", ( (link_map*)p_extra_info )->l_addr );
            printf( "extra_info.l_name: %s\n", ( (link_map*)p_extra_info )->l_name );
            printf( "extra_info.l_ld: %p\n", ( (link_map*)p_extra_info )->l_ld );
            printf( "extra_info.l_next: %p\n", ( (link_map*)p_extra_info )->l_next );
            printf( "extra_info.l_prev: %p\n", ( (link_map*)p_extra_info )->l_prev );
         }         
      }

      if( !nsyms )
         init_instrumental( );

      for( int i = 0; i < nsyms; ++i )
      {
         auto& elf64_sym = elf64_syms[ i ];

         if( ELF64_ST_TYPE( elf64_sym.st_info ) != STT_FUNC )
            continue;
         if( ( elf64_sym.st_value + global_offset ) != (Elf64_Addr)this_fn )
            continue;

         if( depth + 1 >= MAX_DEPTH )
         {
            printf( "MAX_DEPTH(%zu) overlimit\n", MAX_DEPTH );
            break;
         }

         call_chain[ depth + 1 ] = &strtab[ elf64_sym.st_name ];
         break;
      }

      {
         uid_t uid, euid, ruid;
         getresuid( &ruid, &euid, &uid );
         printf( "~ [%04d|%04d|%04d] ", ruid, euid, uid );

         for( std::size_t i = 0; i < depth; ++i )
            printf( "  |" );
         printf( "--" );

         printf( "%s -> %s [depth=%zu];\n", call_chain[ depth ], call_chain[ depth + 1 ], depth );
      }

      ++depth;
   }

   void __cyg_profile_func_exit( void* this_fn, void* call_site )
   {
      // printf( "\nEXIT:  %p, --> %p: %lld\n", this_fn, call_site, __rdtsc( ) );

      --depth;
   }
}
