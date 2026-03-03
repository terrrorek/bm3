#ifndef GAME_EDEF_H
#define GAME_EDEF_H
#include <common/define.h>
#include <cglm/cglm.h>

#define ENT_MAX_FRAMES 32

typedef struct ent_transform_s
{
   vec2  position; // may be encoded as int (2 words)
   float rot; // may be encoded as byte (0 - 255 rots)
}
ent_transform_t;

typedef struct ent_frame_s
{
   uint              gametick;
   ent_transform_t   transform;
}
ent_frame_t;

#endif
