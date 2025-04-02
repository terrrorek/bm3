#include "cvar.hh"

void CVar::Register( void )
{
   if( engFuncs )
      engFuncs->Cvar_Register( &cv );
}

CVar::CVar( const char* name, int number )
{
   cv.flags = CVAR_NUMBER;
   cv.number = number;
}

CVar::CVar( const char* name, float normal )
{
   cv.flags = CVAR_NORMAL;
   cv.normal = normal;
}

CVar::CVar( const char* name, const char* string )
{
   cv.flags = CVAR_STRING;
   BM_strncpy( cv.string, string, sizeof(cv.string) );
}
