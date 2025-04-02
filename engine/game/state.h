#ifndef STATE_H
#define STATE_H

#include <common/common.h>

typedef enum game_state_e
{
   GS_MENU = 0x1,
   GS_GAME = 0x2,
}
game_state_t;

extern game_state_t  gameState;

void GS_Init( void );
void GS_BeginLocal( void );
void GS_End( void );

#endif
