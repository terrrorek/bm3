#ifndef EDECL_H
#define EDECL_H

#include <common/common.h>

// EDECL's are class definitions.
// They hold the class name and class functions.

typedef struct edecl_clstruct_s
{
   uint size;
   uint offset;
}
edecl_clstruct_t;

typedef void               (*entfunc_t)( void* );
typedef void               (*updateentfunc_t)( void*, bool );
typedef edecl_clstruct_t   (*newentfunc_t)( void**, struct eobj_s* );

typedef struct edecl_s
{
   char              name[32];
   edecl_clstruct_t  structure;

   newentfunc_t      create;
   entfunc_t         spawn;
   updateentfunc_t   update;
   entfunc_t         kill;
} edecl_t;

#define MAX_EDECL BIT(8)
extern edecl_t edecls[MAX_EDECL];

edecl_t* Ent_DeclEnt( const char* name );
edecl_t* Ent_GetDecl( const char* name );

#endif
