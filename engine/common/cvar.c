#include "cvar.h"

cvar_t*  last_cvar = NULL;

void CVar_Register( cvar_t* cv )
{
   cv->prev = last_cvar;
   last_cvar = cv;
}

cvar_t* CVar_Find( const char* name )
{
   cvar_t* cv = last_cvar;

   while( cv )
   {
      if( strncmp( cv->name, name, sizeof(cv->name) ) == 0 )
         return cv;
      cv = cv->prev;
   }

   return NULL;
}

float CVar_GetFloat( const char* name )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return 0.f;
   return cv->data.real;
}
uint CVar_GetInt( const char* name )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return 0;
   return cv->data.integer;
}
char* CVar_GetString( const char* name, uint* length )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return NULL;

   *length = cv->data_length;
   return cv->data.string;
}
byte* CVar_GetBuffer( const char* name, uint* length )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return NULL;

   *length = cv->data_length;
   return cv->data.buffer;
}

void CVar_SetFloat( const char* name, float value )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return;
   if( cv->kind != CVARK_REAL ) return;
   cv->data.real = value;
}
void CVar_SetInt( const char* name, uint value )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return;
   if( cv->kind != CVARK_INTEGER ) return;
   cv->data.integer = value;
}
void CVar_SetString( const char* name, char* value, uint length )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return;
   if( cv->kind != CVARK_STRING ) return;
   cv->data.string = value;
   cv->data_length = length;
}
void CVar_SetBuffer( const char* name, byte* value, uint length )
{
   cvar_t* cv = CVar_Find( name );
   if( !cv ) return;
   if( cv->kind != CVARK_BUFFER ) return;
   cv->data.buffer = value;
   cv->data_length = length;
}
