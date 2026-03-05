#ifndef COMMON_CVAR_H
#define COMMON_CVAR_H

#include <common/define.h>

typedef enum cvar_kind_e
{
   CVARK_REAL = 0x0,
   CVARK_STRING = 0x1,
   CVARK_INTEGER = 0x2,
   CVARK_BUFFER = 0x3,
   CVARK_FUNC = 0x4,
}
cvar_kind_t;
typedef enum cvar_flags_e
{
   CVARF_SAVE = 0x1,
}
cvar_flags_t;

typedef void   (*cvarfunc_t)( char* );
typedef struct cvar_s
{
   char           name[64];
   cvar_kind_t    kind;
   cvar_flags_t   flags;
   bool           changed;

   union
   {
      float       real;
      uint        integer;
      char*       string;
      byte*       buffer;
      cvarfunc_t  func;
   }     data;
   uint  data_length;

   struct cvar_s* prev;
}
cvar_t;

void     CVar_Register( cvar_t* cv );
cvar_t*  CVar_Find( const char* name );

float    CVar_GetFloat( const char* name );
uint     CVar_GetInt( const char* name );
char*    CVar_GetString( const char* name, uint* length );
byte*    CVar_GetBuffer( const char* name, uint* length );

void     CVar_SetFloat( const char* name, float value );
void     CVar_SetInt( const char* name, uint value );
void     CVar_SetString( const char* name, char* value, uint length );
void     CVar_SetBuffer( const char* name, byte* value, uint length );

#endif
