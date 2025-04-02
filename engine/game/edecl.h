#ifndef EDECL_H
#define EDECL_H

#include <common/common.h>

typedef void  (*entfunc_t)( void* );
typedef void  (*updateentfunc_t)( void*, bool );
typedef void  (*newentfunc_t)( void**, struct eobj_s* );

// EDECL's are class definitions.
// They hold the class name and class functions.
typedef struct edecl_s
{
   char              name[32];
   newentfunc_t      create;
   entfunc_t         spawn;
   updateentfunc_t   update;
   entfunc_t         kill;
   entfunc_t         destroy;
} edecl_t;

#define MAX_EDECL BIT(8)
extern edecl_t edecls[MAX_EDECL];

edecl_t* Ent_DeclEnt( const char* name );
edecl_t* Ent_GetDecl( const char* name );

#endif
