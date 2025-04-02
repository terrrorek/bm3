
#ifndef ALLOC_H
#define ALLOC_H

#include <common/common.h>

/*
 * === BM Memory Allocator ===
 *  The allocator works on a heap allocated block allocated at the beginning of the program.
 *  The block is split into sectors, which contain the actual data.
 */

typedef enum msector_flags_e
{
   MSEC_SPLITABLE = 0x1,
}
msector_flags_t;

typedef struct msector_s
{
   uint32_t size;       // sector size
   uint8_t  flags;      // sector flags
   void*    data;       // sector data pointer

   struct msector_s* prev, * next;
}
msector_t;

void  Mem_Print( void );
void  Mem_Init( void* heap, int heapsize );
void* Mem_Alloc( uint32_t size );
void  Mem_Free( void* data );

#endif
