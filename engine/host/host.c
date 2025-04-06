
#include <host/host.h>
#include <net/net.h>
#include <common/cmd.h>
#include <client/client.h>
#include <game/state.h>

void Host_Init( void )
{

}

void Host_Update( void )
{
   if( !gameLocal && !isDedicated )
      return;

   Net_ServerLoop( &hostsock );
   Ent_Process( false );
}

void Host_Quit( void )
{

}
