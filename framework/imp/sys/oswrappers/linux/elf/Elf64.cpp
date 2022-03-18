#include "api/sys/oswrappers/linux/elf/Elf64.hpp"



using namespace carpc::os::os_linux;



Elf64::Elf64( )
   : app( )
{
}

Elf64::~Elf64( )
{
   free( elf64_syms );
   free( strtab );
}

bool Elf64::init( )
{
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
         fill_tab( &strtab );
      }
      if( elf64_shdr.sh_type == SHT_SYMTAB )
      {
         r = fill_tab( (char**)&elf64_syms );
         nsyms = r / sizeof( Elf64_Sym );
      }
      done = elf64_syms != nullptr && strtab != nullptr;
   }

   free( shstrtab );

   app.close( );

   return true;
}

int Elf64::fill_tab( char** result )
{
   off_t offset = app.lseek( 0, SEEK_CUR );
   if( (off_t)-1 == offset )
      return -1;

   if( (off_t)-1 == app.lseek( elf64_shdr.sh_offset, SEEK_SET ) )
      return -1;

   free( *result );
   *result = (char*)malloc( elf64_shdr.sh_size );
   int return_result = app.read( *result, elf64_shdr.sh_size );

   if( offset != app.lseek( offset, SEEK_SET ) )
      return -1;

   return return_result;
}

bool Elf64::is_operable( ) const
{
   return 0 != nsyms;
}
