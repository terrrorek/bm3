#ifndef GAME_H
#define GAME_H
#include <common/define.h>

typedef struct game_state_s
{
   bool  vm;
   union
   {
      void* dll;
      /* TODO: integrate bmvm */
   }
   handle;
}
game_state_t;

extern game_state_t game;

void  Game_Init( bool vm );
void  Game_Quit( void );
void  Game_Update( void );
void* Game_GetSymbol( const char* name );

#endif
