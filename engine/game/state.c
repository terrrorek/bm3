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
   gameLocal = FALSE;
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
   if( Net_Connect( &gamesock, addr, port ) )
      gameState = GS_GAME;
}

void Game_ConnectLocal( void )
{
   double   wait = 0.0;

   gameLocal = TRUE;

   Game_Host( );
   while( wait <= 0.5 ) wait += gameTime;
   Game_Connect( "127.0.0.1", 21371 );
}

void Game_Exit( void )
{
   if( gameLocal )
      Game_EndHost( );

   Net_Disconnect( &gamesock );
   gameState = GS_MENU;
}
