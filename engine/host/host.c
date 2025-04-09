
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
   if( gameState == GS_GAME )
      Ent_Process( false );

   Net_ServerLoop( );
}

void Host_Quit( void )
{
   Net_EndHost( );
}
