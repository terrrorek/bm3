
#ifndef CVAR_H
#define CVAR_H

#include <common/common.h>

typedef enum cvar_flags_e
{
   CVAR_NORMAL       = 0x0001,
   CVAR_NUMBER       = 0x0002,
   CVAR_STRING       = 0x0004,
   CVAR_FUNCTION     = 0x0008,

   CVAR_REGISTERED   = 0x1000,

   CVAR_INTERNAL     = 0x0100,
   CVAR_LOCKED       = 0x0200,
   CVAR_SERVER       = 0x0400,
}
cvar_flags_t;

typedef void (*confunc_t)(const char*);

typedef struct cvar_s
{
   char           name[48];
   cvar_flags_t   flags;

   union
   {
      char        string[200];
      int         number;
      float       normal;
      confunc_t   function;
   };

   bool           changed;
   struct cvar_s* next, * prev;
}
cvar_t;

extern cvar_t* lastCVar;

void     CVar_Register( cvar_t* cvar );
cvar_t*  CVar_Get( const char* name );

#endif
