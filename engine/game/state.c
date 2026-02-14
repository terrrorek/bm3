#include <game/state.h>
#include <game/eobj.h>
#include <common/cvar.h>
#include <game/console.h>
#include <video/vid.h>
#include <video/rend.h>
#include <net/net.h>

game_state_t   gameState = GS_MENU;
bool           gameLocal = FALSE;

eobj_t*  player = 0;

static void Game_SwitchState_f( const char* state )
{
   if( !state )
      return;
   if( strncmp( state, "menu", 4 ) == 0 )
   {
      gameLocal = FALSE;
      gameState = GS_MENU;
   }
   else if( strncmp( state, "ingame", 6 ) == 0 )
   {
      gameLocal = TRUE;
      gameState = GS_INGAME;
   }
}
static cvar_t g_switchstate = {
   .name = "g_switchstate",
   .flags = CVAR_FUNCTION,
};

static void Game_Connect_f( const char* ip )
{
   if( !ip )
   {
      Con_Errorf( "g_connect: no ip given!" );
      return;
   }
   if( strncmp( ip, "local", 5 ) == 0 )
   {
      gameLocal = TRUE;
      gameState = GS_CONNECTING;
      return;
   }

   uint16_t port;
   char* port_point = strchr( ip, ':' );

   if( !port_point )
      port = NET_PORT;
   else
   {
      port = strtoll( port_point+1, NULL, 10 );
      *port_point = 0;
   }

   Game_Connect( ip, port );
}
static cvar_t g_connect = {
   .name = "g_connect",
   .flags = CVAR_FUNCTION,
};

void Game_Init( void )
{
   g_switchstate.function = (confunc_t) Game_SwitchState_f;
   CVar_Register( &g_switchstate );
   g_connect.function = (confunc_t) Game_Connect_f;
   CVar_Register( &g_connect );
}

void Game_Exit( void )
{

}

void Game_Host( void )
{

}

void Game_EndHost( void )
{

}

void Game_Draw( void )
{
   switch( gameState )
   {
   case GS_INGAME:
      Ent_Draw( );
      break;

   case GS_CONNECTING:
      rendImports.Rend_DrawPic( conBackground, 0, 0, vidState.width, vidState.height );
      rendImports.Rend_DrawString(
         conFont,
         vidState.width/2 - (int)(7*6.5),
         vidState.height/2,
         7, 9, "connecting..."
      );
      Vid_Draw( );
      return;

   case GS_CONNECTED:
   default:
      break;
   }

   Con_Draw( );
   Vid_Draw( );
}

void Game_Connect( const char* addr, uint16_t port )
{
   printf( "%s:%u\n", addr, port );
}

void Game_Disconnect( void )
{

}
