#pragma once



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
