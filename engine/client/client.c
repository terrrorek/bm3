
#include <client/client.h>
#include <net/msg.h>
#include <common/cmd.h>

bool        clientConnected = false;
netchan_t   clientChan = {0};

void Client_Init( void )
{
   Chan_OpenChannel( &clientChan.chan, 0 );
}

void Client_Update( void )
{
   if( !clientConnected )
      return;
   Net_ClientLoop( );
}

void Client_Quit( void )
{
   Chan_CloseChannel( &clientChan.chan );
}
