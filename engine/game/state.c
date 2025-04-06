#include <game/state.h>
#include <common/bmstring.h>
#include <game/console.h>
#include <common/cmd.h>
#include <net/net.h>
#include <common/cvar.h>
#include <game/eobj.h>
#include <video/pic.h>

#include <SDL3/SDL.h>

bool     isTrying = FALSE;
uint     tries = 0;
char     tryAddr[32];
uint16_t tryPort;

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

void Game_Init( void )
{
   CVar_Register( &_connect );
   CVar_Register( &_disconnect );
}

void Game_Host( void )
{
   Net_Host( );
}

void Game_EndHost( void )
{
   Net_EndHost( );
}

void Game_TryConnect( void )
{
   if( !isTrying )
      return;

   switch( isTrying )
   {
   case 1:
      if( tries > 5 )
      {
         isTrying = FALSE;
         tries = 0;
         Con_Errorf( "failed to connect to %s:%i!\n", tryAddr, tryPort );
         tryAddr[0] = '\0';
         tryPort = 0;
         return;
      }

      if(!Net_Connect( &gamesock, tryAddr, tryPort ))
      {
         Con_Printf( "again trying connect to %s:%i... (try %i)\n",
            tryAddr, tryPort, 1 + tries++ );
         return;
      }
      else
      {
         connectTime = (double) SDL_GetTicks( ) / 1000.0;
         Con_Printf( "connected to %s:%i.\n", tryAddr, tryPort );
         gameState = GS_GAME;
         isTrying = FALSE;
         Con_Printf( "waiting for acceptance...\n" );
      }
      break;
   default: break;
   }
}

void Game_Connect( const char* addr, uint16_t port )
{
   Net_DestroySocket( &gamesock );
   Net_InitSocket( &gamesock );

   isTrying = TRUE;
   BM_strcpy( tryAddr, addr );
   tryPort = port;
}

void Game_ConnectLocal( void )
{
   gameLocal = TRUE;
   Game_Host( );
   Game_Connect( "127.0.0.1", 21371 );
}

void Game_Exit( void )
{
   gameState = GS_MENU;
   Net_EndHost( );
   Net_DestroySocket( &gamesock );
}
