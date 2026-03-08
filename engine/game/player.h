#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <common/define.h>
#include <game/econ.h>

#define GAME_MAX_PLAYERS      16
#define GAME_MAX_PLAYER_NAME  64

typedef struct playercmd_s
{
   float vert, hor;
}
playercmd_t;

typedef struct player_s
{
   char        name[GAME_MAX_PLAYER_NAME];
   uint32_t    id;
   econ_t*     ent;
   byte        current_cmd;
   playercmd_t cmds[ENT_MAX_FRAMES];
}
player_t;

#endif
