#pragma once



#if defined(__x86_64__) || defined(_M_X64)
   #define _ARCH_x86_64_CPU_
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
   #define _ARCH_x86_32_CPU_
#elif defined(__ARM_ARCH_2__)
   #define _ARCH_ARM2_CPU_
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
   #define _ARCH_ARM3_CPU_
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
   #define _ARCH_ARM4T_CPU_
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
   #define _ARCH_ARM5_CPU_
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
   #define _ARCH_ARM6T2_CPU_
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
   #define _ARCH_ARM6_CPU_
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
   #define _ARCH_ARM7_CPU_
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
   #define _ARCH_ARM7A_CPU_
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
   #define _ARCH_ARM7R_CPU_
#elif defined(__ARM_ARCH_7M__)
   #define _ARCH_ARM7M_CPU_
#elif defined(__ARM_ARCH_7S__)
   #define _ARCH_ARM7S_CPU_
#elif defined(__aarch64__) || defined(_M_ARM64)
   #define _ARCH_ARM64_CPU_
#elif defined(mips) || defined(__mips__) || defined(__mips)
   #define _ARCH_MIPS_CPU_
#elif defined(__sh__)
   #define _ARCH_SUPERH_CPU_
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
   #define _ARCH_POWERPC_CPU_
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
   #define _ARCH_POWERPC64_CPU_
#elif defined(__sparc__) || defined(__sparc)
   #define _ARCH_SPARC_CPU_
#elif defined(__m68k__)
   #define _ARCH_M68K_CPU_
#else
   #define _ARCH_UNKNOWN_CPU_
#endif



#include <climits>
#include <cstdint>

#if CHAR_BIT != 8
   #error "unsupported char size"
#endif



// https://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
namespace carpc {

   enum class eByteOrder : std::uint32_t
   {
      O32_LITTLE_ENDIAN    = 0x03020100ul,                                                   // 00000011 00000010 00000001 00000000
      O32_BIG_ENDIAN       = 0x00010203ul,                                                   // 00000000 00000001 00000010 00000011
      O32_PDP_ENDIAN       = 0x01000302ul,      /* DEC PDP-11 (aka ENDIAN_LITTLE_WORD) */    // 00000001 00000000 00000011 00000010
      O32_HONEYWELL_ENDIAN = 0x02030001ul /* Honeywell 316 (aka ENDIAN_BIG_WORD) */          // 00000010 00000011 00000000 00000001
   };

   const char* const to_str( const eByteOrder& value );

   static const union
   {
      unsigned char bytes[4];
      std::uint32_t value;
   } o32_host_order = { { 0, 1, 2, 3 } };

   const eByteOrder byte_order( );

}

#define O32_HOST_ORDER ( o32_host_order.value )

#if 0 // Example

   int main( )
   {
      printf( "%s", carpc::to_str( carpc::byte_order( ) ) );

      return 0;
   }

#endif

