
#include <client/client.h>
#include <common/cmd.h>
#include <host/host.h>
#include <game/state.h>
#include <net/net.h>
#include <common/bmstring.h>
#include <game/console.h>

extern uint64_t secondDur;

bool     accepted = FALSE;

void Client_Init( void )
{

}

void Client_Update( void )
{
   Net_ClientLoop( &gamesock );

   if(!( gamesock.flags & SOCK_CONNECTED ))
      return;

   Ent_Process( true );
}

void Client_Quit( void )
{

}
