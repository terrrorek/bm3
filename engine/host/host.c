
#include <host/host.h>
#include <net/net.h>
#include <net/msg.h>
#include <game/state.h>
#include <common/cmd.h>

netchan_t hostChan = {0};

void Host_Init( void )
{
   Chan_OpenBoundChannel( &hostChan.chan, 0, (addr_t){.base = 0, .port = NET_PORT} );
}

void Host_Update( void )
{
   if( !gameLocal )
      return;
   Net_HostLoop( );
}

void Host_Quit( void )
{
   Chan_CloseChannel( &hostChan.chan );
}
