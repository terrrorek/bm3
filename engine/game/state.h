#ifndef STATE_H
#define STATE_H

#include <common/common.h>

typedef enum game_state_e
{
   GS_MENU = 0x1,
   GS_GAME = 0x2,
}
game_state_t;

extern double        gameTime, connectTime;
extern game_state_t  gameState;
extern bool          gameLocal;

void Game_Init( void );
void Game_Host( void );
void Game_EndHost( void );
void Game_Connect( const char* addr, uint16_t port );
void Game_ConnectLocal( void );
void Game_Exit( void );

#endif
