#ifndef EOBJ_H
#define EOBJ_H

#include <common/common.h>
#include <common/bmmath.h>
#include <game/edecl.h>
#include <game/emat.h>

#undef bool

// User defined data
typedef struct edata_s
{
   bool        spawned;
   char        name[64];
   transform_t transform;
} edata_t;

// EOBJ's are the entities themselves.
// They hold position data, clazz pointers, edecl pointers and stuff.
typedef struct eobj_s
{
   edecl_t*    edecl;
   void*       clazz;

   // If the entity causes an soft error, it's chances counter will
   // go up. If the counter bypasses a certain value, the entity will be destroyed.
   byte        chances;
   uint        index;
   byte        player;
   emat_t      mat;

   edata_t     data;
   edata_t     lastdata; // Data before update
   float       interp;
} eobj_t;

#define MAX_EOBJ_CHANCES 3

#define MAX_EOBJ BIT(10)
extern eobj_t eobjs[MAX_EOBJ];

eobj_t* Ent_Create( const char* classname );
void Ent_Update( eobj_t* ent, bool client );
bool Ent_Destroy( eobj_t* ent );
void Ent_DestroyAll( void );
void Ent_Process( bool client );
void Ent_Draw( void );
uint Ent_Count( void );

#endif
