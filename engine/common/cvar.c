
#include "cvar.h"
#include <common/bmstring.h>
#include <common/cmd.h>

cvar_t* lastCVar = NULL;

void CVar_Register( cvar_t* cvar )
{
   cvar_t*  found;

   if(( found = CVar_Get(cvar->name) ))
      return;

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
      if(!( cvar->flags & CVAR_INTERNAL ) && !BM_strcmp( cvar->name, name ))
         return cvar;
      cvar = cvar->prev;
   }
   return NULL;
}
