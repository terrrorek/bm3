#ifndef GAME_ECON_H
#define GAME_ECON_H
#include <common/define.h>
#include <game/edef.h>

struct econ_s;

typedef struct eclass_s
{
   void  (*Spawn)( struct econ_s* );
   void  (*Update)( struct econ_s* );
   void  (*End)( struct econ_s* );
}
eclass_t;

/* entity container */
typedef struct econ_s
{
   char              classname[32];
   eclass_t          eclass;
   ent_frame_t       frames[ENT_MAX_FRAMES];
   byte              frame_index;
}
econ_t;

#endif
