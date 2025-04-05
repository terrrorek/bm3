
#include "alloc.h"
#include <common/cmd.h>
#include <common/args.h>

typedef struct mheader_s
{
   msector_t*  sector; // first sector
}
mheader_t;

static void*      HeapBlock = NULL, * HeapMax = NULL;
static int        BlockSize = 0;
static mheader_t* MemHeader = NULL;

#if 0
static msector_t* Mem_FindLargestSector( void )
{
   msector_t*  sector, * largest;

   for( largest = sector = MemHeader->sector;
      sector; sector = sector->next )
   {
      if( sector->size > largest->size && sector->flags & MSEC_SPLITABLE )
         largest = sector;
   }

   return largest;
}

static msector_t* Mem_SplitSector( msector_t* sector, uint size )
{
   msector_t*  new;

   new = NULL;
   if( !(sector->flags & MSEC_SPLITABLE) )
      return new;

   if( sector->size <= size )
      return new;

   sector->size -= size;

   new = (msector_t*)( (uintptr_t)sector->data + sector->size - sizeof(msector_t) );
   WIPEPTR( new );

   new->size = size;
   new->data = (void*)( (uintptr_t)new + sizeof(msector_t) );

   if( sector->next )
   {
      sector->next->prev = new;
      new->next = sector->next;
   }
   sector->next = new;
   new->prev = sector;

   return new;
}
#endif

void Mem_Print( void )
{
   msector_t*  sector;

   for( sector = MemHeader->sector; sector; sector = sector->next )
   {
      Cmd_Debugf( "\tSECTOR %p:\n"
                  "\t   size: %i\n"
                  "\t   flags: %02x\n"
                  "\t   prev sector: %p\n"
                  "\t   next sector: %p\n",
         sector, sector->size, sector->flags, sector->prev, sector->next );
   }
}

void Mem_Init( void* heap, int heapsize )
{
   msector_t*  sector;

   HeapBlock = heap, BlockSize = heapsize;
   HeapMax = (void*)( ((uintptr_t)HeapBlock + BlockSize) );

   // clear heap
   memset( HeapBlock, 0, BlockSize );

   // create header
   MemHeader = (mheader_t*) HeapBlock;
   HeapBlock = (void*)( (uintptr_t)HeapBlock + sizeof(mheader_t) );

   sector = (msector_t*) HeapBlock;
   MemHeader->sector = sector;
   WIPEPTR( sector );

   sector->flags = MSEC_SPLITABLE;
   sector->size = BlockSize - sizeof(mheader_t);
   sector->data = (void*)( (uintptr_t)sector + sizeof(msector_t) );

   Mem_Print( );
}

void* Mem_Alloc( uint32_t size )
{
   return malloc( size );
   #if 0
   msector_t*  base, * sector;

   base = Mem_FindLargestSector( );
   if( !base )
      Cmd_Errorf( "alloc failure. no sectors available.\n" );

   sector = Mem_SplitSector( base, size );
   if( !sector )
      Cmd_Errorf( "alloc failure. failed to split largest sector (size %i).\n", size );

   return sector->data;
   #endif
}

void* Mem_Realloc( void* data, uint32_t size )
{
   return realloc( data, size );
   #if 0

   #endif
}

void Mem_Free( void* data )
{
   free( data );
   #if 0
   msector_t*  sector;

   sector = (msector_t*)( (uintptr_t)data - sizeof(msector_t) );

   if( (uintptr_t)sector > (uintptr_t)HeapBlock &&
       (uintptr_t)sector < (uintptr_t)HeapMax )
   {
      if( sector->next )
         sector->next->prev = sector->prev;
      if( sector->prev )
         sector->prev->next = sector->next;

      memset( sector->data, 0, sector->size );
      WIPEPTR( sector );
   }
   #endif
}
