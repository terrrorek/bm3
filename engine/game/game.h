#ifndef GAME_H
#define GAME_H

#include <common/define.h>
#include <common/cvar.h>
#include <game/player.h>
#include <bmvm/vm/bmvm.h>

typedef enum gametype_e
{
   GAME_MENU = 0,
   GAME_CONNECTING = 1,
   GAME_CONNECTED = 2
}
gametype_t;

typedef enum gamecon_e
{
   GAMECON_LOOPBACK = 0,
   GAMECON_NET = 1,
}
gamecon_t;

typedef struct gamestate_s
{
   bool  vm;
   union
   {
      void*          dll;
      /* TODO: integrate bmvm */
      bmvm_state_t   vm;
   }
   handle;

   gametype_t  type;
   gamecon_t   con;
   player_t    players[GAME_MAX_PLAYERS];
}
gamestate_t;

extern gamestate_t   game;
extern cvar_t        game_rate;
extern double        last_update, update_time;

void  Game_Init( bool vm );
void  Game_Quit( void );
void* Game_GetSymbol( const char* name );

void  Game_UpdateSim( void );

#endif
