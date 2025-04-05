
#include <client/client.h>
#include <common/cmd.h>
#include <host/host.h>
#include <game/state.h>
#include <net/net.h>

extern uint64_t secondDur;

void Client_Init( void )
{

}

void Client_Update( void )
{
   if(!( gamesock.flags & SOCK_CONNECTED ))
      return;

   Net_ClientLoop( &gamesock );
   Ent_Process( true );
}

void Client_Quit( void )
{

}
