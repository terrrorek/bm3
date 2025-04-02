#ifndef CVAR_HH
#define CVAR_HH

#include "game.hh"
#include <common/common.h>
#include <engine/common/cvar.h>

#define REG_CHECK(x)\
   if(!( x.cv.flags & CVAR_REGISTERED ))\
      x.Register( )

class CVar
{
public:
   cvar_t   cv;

   inline CVar( ) {}

   CVar( const char* name, int number );
   CVar( const char* name, float normal );
   CVar( const char* name, const char* string );

   void Register( void );
};

#define CVAR(name, value)\
   CVar name( BMSTR(name), value )

#endif
