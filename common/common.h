
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#undef bool

#define BMSTR(x) __STRINGIFY(x)
#define __STRINGIFY(x) #x

#define TRUE 1
#define FALSE 0

#define MAKE_VERSION(major, minor) (uint16_t)(( major << 8 ) | minor)
#define WIPEOUT(x) memset( &x, 0, sizeof(x) )
#define WIPEPTR(x) memset( x, 0, sizeof(*x) )
#define BIT(x) (1 << x)

#define GAMENAME BM
#define GAMEVERSION MAKE_VERSION(0, 1)

#define TYPEDEF_SIZE(type, size) typedef type##_t type##size##_t[size]

typedef unsigned char   byte;
#ifndef __cplusplus
typedef byte            bool;
#endif
typedef unsigned int    uint;

static void HexDump( void* ptr, uint size )
{
   uint  i, n;

   for( i = 0, n = 1; i < size; i++ )
   {
      if( n % 9 == 0 )
      {
         n = 1;
         printf( "\n" );
      }

      printf( "%02x ", ((byte*)ptr)[i] );
      ++n;
   }
}

#endif
