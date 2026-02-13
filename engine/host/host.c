
#include <host/host.h>
#include <common/cmd.h>
#include <client/client.h>
#include <game/state.h>
#include <net/net.h>

void Host_Init( void )
{
}

void Host_Update( void )
{
   if( gameState == GS_GAME && gameLocal )
      Ent_Process( false );

   if( net_server.bound )
      SNet_Update();
}

void Host_Quit( void )
{
}
