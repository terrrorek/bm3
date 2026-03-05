#include <server/server.h>
#include <game/game.h>

netchan_t   sv_netchan;
cvar_t      dedicated = {"dedicated", CVARK_INTEGER, 0, false, {.integer = 0}, 0, NULL};
uint        sv_gametick = 0;

void SV_Init( void )
{
   byte addr[4] = {0};
   Netchan_Init( &sv_netchan, NETCHAN_NET, addr, NET_DEFAULT_PORT );
}

void SV_Quit( void )
{
   Netchan_Quit( &sv_netchan );
}

void SV_Update( void )
{
   if( game.type != GAME_CONNECTED )
      return;

   Game_UpdateSim( );
}
