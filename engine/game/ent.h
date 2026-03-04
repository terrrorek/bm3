#ifndef GAME_ENT_H
#define GAME_ENT_H
#include <common/define.h>
#include <game/econ.h>

#define ENT_MAX_ENTITIES 16384
extern econ_t  game_entities[ENT_MAX_ENTITIES];

#define EXPORT_ECLASS(classname, eclass)\
   DLL_EXPORT eclass_t* __##classname##_eclass( void ) { return &eclass; }

econ_t*     Ent_Allocate( const char* classname );
void        Ent_Free( econ_t* ent );

#endif
