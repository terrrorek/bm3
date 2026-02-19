#ifndef COMMON_ALLOC_H
#define COMMON_ALLOC_H

#include <common/define.h>
#include <stdlib.h>

static inline void* Mem_Alloc( size_t size )
{
   return calloc( 1, size );
}
static inline void  Mem_Free( void* ptr )
{
   free( ptr );
}

#endif
