#ifndef COMMON_ARCH_H
#define COMMON_ARCH_H

#if defined(__x86_64__) || defined(_M_X64)
#define CPU_ARCH "x86_64"
#define DLL_ARCH "x64"
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define CPU_ARCH "x86_32"
#define DLL_ARCH "x32"
#elif defined(__ARM_ARCH_2__)
#define CPU_ARCH "ARM2"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
#define CPU_ARCH "ARM3"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
#define CPU_ARCH "ARM4T"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
#define CPU_ARCH "ARM5"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
#define CPU_ARCH "ARM6T2"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#define CPU_ARCH "ARM6"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define CPU_ARCH "ARM7"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define CPU_ARCH "ARM7A"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
#define CPU_ARCH "ARM7R"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_7M__)
#define CPU_ARCH "ARM7M"
#define DLL_ARCH CPU_ARCH
#elif defined(__ARM_ARCH_7S__)
#define CPU_ARCH "ARM7S"
#define DLL_ARCH CPU_ARCH
#elif defined(__aarch64__) || defined(_M_ARM64)
#define CPU_ARCH "ARM64"
#define DLL_ARCH CPU_ARCH
#elif defined(mips) || defined(__mips__) || defined(__mips)
#define CPU_ARCH "MIPS"
#define DLL_ARCH "mips"
#elif defined(__sh__)
#define CPU_ARCH "SUPERH"
#define DLL_ARCH "superh"
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
#define CPU_ARCH "POWERPC"
#define DLL_ARCH "ppc"
#elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#define CPU_ARCH "POWERPC64"
#define DLL_ARCH "ppc64"
#elif defined(__sparc__) || defined(__sparc)
#define CPU_ARCH "SPARC"
#define DLL_ARCH "sparc"
#elif defined(__m68k__)
#define CPU_ARCH "M68K"
#define DLL_ARCH "m68k"
#else
#define CPU_ARCH "UNKNOWN"
#endif

#endif
