#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <link.h>
#include <elf.h>
#include "Application.hpp"



namespace instrumental {

   struct Elf64
   {
      Elf64( );
      ~Elf64( );

      bool init( ) __attribute__(( no_instrument_function ));
      int fill_tab( char** result ) __attribute__(( no_instrument_function ));
      bool is_operable( ) const __attribute__(( no_instrument_function ));

      Elf64_Ehdr elf64_ehdr;
      Elf64_Shdr elf64_shdr;
      Elf64_Sym* elf64_syms = nullptr;
      char* strtab = nullptr;
      int nsyms = 0;

      Application app;
   };

}
