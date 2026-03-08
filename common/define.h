#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H

#undef bool
#undef false
#undef true

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <common/arch.h>

typedef unsigned  uint;
typedef uint8_t   byte;

#undef bool
#ifndef __cplusplus
   typedef  byte bool;
   enum     { false, true };
#else
   typedef  byte __bool;
   #define  bool __byte
#endif

#define WIPE(x)\
   memset( &x, 0, sizeof(x) )

extern bool running;

#endif
