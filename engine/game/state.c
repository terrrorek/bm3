#include <game/state.h>
#include <common/bmstring.h>
#include <game/console.h>
#include <common/cmd.h>
#include <common/cvar.h>
#include <game/eobj.h>
#include <video/pic.h>
#include <net/net.h>

#include <SDL3/SDL.h>

double   connectTime = 0.0;
bool     gameLocal = FALSE;

extern eobj_t* player;

static void Game_Connect_f( const char* address )
{
   if( address == NULL )
   {
      Con_Printf( "no address given.\n" );
      return;
   }

   int      port;
   char     addr[32];

   char*    sep = strchr( address, ':' );
   if( !sep )
      port = 21371;
   else
   {
      port = atoi( sep + 1 );
      *sep = 0;
   }

   BM_strcpy( addr, address );
   Game_Connect( addr, port );
}
static cvar_t _connect = {
   .name = "connect",
   .flags = CVAR_FUNCTION,
   .function = Game_Connect_f
};

static void Game_Disconnect_f( void )
{
   Game_Exit( );
}
static cvar_t _disconnect = {
   .name = "disconnect",
   .flags = CVAR_FUNCTION,
   .function = (confunc_t) Game_Disconnect_f,
};

static void Game_State_f( void )
{
   const char* str;

   switch( gameState )
   {
   case GS_MENU: str = "Menu"; break;
   case GS_GAME: str = "Game"; break;
   default: str = "?";
   }

   Con_Printf( "%s\n", str );
}
static cvar_t _game_state = {
   .name = "game_state",
   .flags = CVAR_FUNCTION,
   .function = (confunc_t) Game_State_f,
};

static void Game_ConnectLocal_f( void )
{
   Game_ConnectLocal( );
}
static cvar_t _con_local = {
   .name = "con_local",
   .flags = CVAR_FUNCTION,
   .function = (confunc_t) Game_ConnectLocal_f
};

void Game_Init( void )
{
   CVar_Register( &_connect );
   CVar_Register( &_disconnect );
   CVar_Register( &_con_local );
   CVar_Register( &_game_state );
}

void Game_Host( void )
{
   Net_Host( );
}

void Game_EndHost( void )
{
   Net_EndHost( );
}

void Game_Connect( const char* addr, uint16_t port )
{
   gameLocal = FALSE;
   if( Net_Connect( &gamesock, addr, port ) )
      gameState = GS_GAME;
}

void Game_ConnectLocal( void )
{
   gameLocal = TRUE;
   gameState = GS_GAME;
   player = Ent_Create( "player" );
}

void Game_Exit( void )
{
   Net_Disconnect( &gamesock );
   Ent_DestroyAll( );
   player = 0;
   gameState = GS_MENU;
}
