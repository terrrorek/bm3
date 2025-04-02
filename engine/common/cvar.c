
#include "cvar.h"
#include <common/bmstring.h>
#include <common/cmd.h>

static cvar_t* lastCVar = NULL;

void CVar_Register( cvar_t* cvar )
{
   if( lastCVar )
      lastCVar->next = cvar;
   cvar->prev = lastCVar;
   lastCVar = cvar;
   cvar->flags |= CVAR_REGISTERED;
}

cvar_t* CVar_Get( const char* name )
{
   cvar_t*  cvar = lastCVar;

   while( cvar )
   {
      if(!BM_strcmp( cvar->name, name ))
         return cvar;
      cvar = cvar->prev;
   }
   return NULL;
}
