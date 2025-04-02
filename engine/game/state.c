#include <game/state.h>
#include <game/console.h>
#include <common/cmd.h>
#include <net/net.h>
#include <common/cvar.h>
#include <game/eobj.h>
#include <video/pic.h>

cvar_t   game_begin_f = {
   .name = "game_begin",
   .function = GS_BeginLocal,
   .flags = CVAR_FUNCTION,
};
cvar_t   game_end_f = {
   .name = "game_end",
   .function = GS_End,
   .flags = CVAR_FUNCTION,
};

void GS_Init( void )
{
   CVar_Register( &game_begin_f );
   CVar_Register( &game_end_f );
}

void GS_BeginLocal( void )
{
   gameState = GS_GAME;

   Net_Connect( &gamesock, "127.0.0.1", 21371 );

   uint test = R_LoadTexture( "pic.tga", 0 );
   eobj_t* pl = Ent_Create( "player" );
   pl->data.transform.scale[0] = 64.f;
   pl->data.transform.scale[1] = 64.f;
   pl->mat.pic = test;
}

void GS_End( void )
{
   Net_Disconnect( &gamesock );

   Ent_DestroyAll( );
}
