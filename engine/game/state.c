#include <game/state.h>
#include <common/bmstring.h>
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

bool        isTrying = false;
uint        tries = 0;
char        tryAddr[32];
uint16_t    tryPort;

void GS_Init( void )
{
   CVar_Register( &game_begin_f );
   CVar_Register( &game_end_f );
}

void GS_Try( void )
{
   if( !isTrying )
      return;

   if(!Net_Connect( &gamesock, tryAddr, tryPort ))
   {
      if( tries >= 3 )
      {
         Con_Errorf( "stop trying.\n" );
         isTrying = FALSE;
         tries = 0;
         return;
      }

      Con_Printf( "try %i...\n", tries++ );
      return;
   }

   isTrying = 0;
   tries = 0;
}

void GS_Update( void )
{
   GS_Try( );
}

void GS_BeginLocal( void )
{
   gameState = GS_GAME;

   isTrying = TRUE;
   tries = 0;

   BM_strcpy( tryAddr, "127.0.0.1" );
   tryPort = 21371;

   // uint test = R_LoadTexture( "pic.tga", 0 );
   // eobj_t* pl = Ent_Create( "player" );
   // pl->data.transform.scale[0] = 64.f;
   // pl->data.transform.scale[1] = 64.f;
   // pl->mat.pic = test;
}

void GS_End( void )
{
   Net_Disconnect( &gamesock );

   Ent_DestroyAll( );
}
