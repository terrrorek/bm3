
#ifndef CVAR_H
#define CVAR_H

#include <common/common.h>

typedef enum cvar_flags_e
{
   CVAR_NORMAL       = 0x1,
   CVAR_NUMBER       = 0x2,
   CVAR_STRING       = 0x4,
   CVAR_FUNCTION     = 0x8,

   CVAR_REGISTERED   = 0x100
}
cvar_flags_t;

typedef struct cvar_s
{
   char           name[48];
   cvar_flags_t   flags;

   union
   {
      char     string[200];
      int      number;
      float    normal;
      void     (*function)(void);
   };

   bool           changed;
   struct cvar_s* next, * prev;
}
cvar_t;

void     CVar_Register( cvar_t* cvar );
cvar_t*  CVar_Get( const char* name );

#endif
