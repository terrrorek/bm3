#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H

#undef bool
#undef false
#undef true

#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef unsigned  uint;
typedef uint8_t   byte;

#ifndef __cplusplus
   typedef  byte bool;
   enum     { false, true };
#endif

#define WIPE(x)\
   memset( &x, 0, sizeof(x) )

extern bool running;

#endif
