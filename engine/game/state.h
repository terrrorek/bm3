#ifndef STATE_H
#define STATE_H

#include <common/common.h>
#include <common/cvar.h>

typedef enum game_state_e
{
   GS_MENU = 1,
   GS_CONNECTING = 2,
   GS_CONNECTED = 3,
   GS_INGAME = 4
}
game_state_t;

extern cvar_t        g_rate;
extern double        gameTime;
extern game_state_t  gameState;
extern bool          gameLocal;

void Game_Init( void );
void Game_Exit( void );
void Game_Host( void );
void Game_EndHost( void );
void Game_Draw( void );

void Game_Connect( const char* addr, uint16_t port );
void Game_Disconnect( void );

#endif
