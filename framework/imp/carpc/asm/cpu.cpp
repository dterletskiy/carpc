#include "carpc/common/Definitions.hpp"
#include "carpc/asm/cpu.hpp"




unsigned long long __rdtsc__( void )
{
   #if defined(_ARCH_x86_32_CPU_)
      unsigned long long int x;
      __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
      return x;
   #elif defined(_ARCH_x86_64_CPU_)
      unsigned hi, lo;
      __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
      return ( (unsigned long long)lo ) | ( ( (unsigned long long)hi ) << 32 );
   #else
      return 0;
   #endif
}
