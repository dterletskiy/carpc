#include <string.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <link.h>
#include <elf.h>

#include "api/sys/oswrappers/linux/elf/Application.hpp"



namespace carpc::os::os_linux {

   struct Elf64
   {
      Elf64( );
      ~Elf64( );

      bool init( );
      int fill_tab( char** result );
      bool is_operable( ) const;

      Elf64_Ehdr elf64_ehdr;
      Elf64_Shdr elf64_shdr;
      Elf64_Sym* elf64_syms = nullptr;
      char* strtab = nullptr;
      int nsyms = 0;

      Application app;
   };

} // namespace carpc::os::os_linux
