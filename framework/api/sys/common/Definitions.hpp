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
