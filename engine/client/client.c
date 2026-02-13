
#include <client/client.h>
#include <common/cmd.h>
#include <host/host.h>
#include <game/state.h>
#include <common/bmstring.h>
#include <game/console.h>
#include <net/net.h>

extern uint64_t secondDur;

bool     accepted = FALSE;

void Client_Init( void )
{

}

void Client_Update( void )
{
   if( gameState == GS_GAME )
      Ent_Process( true );

   CNet_Update();
}

void Client_Quit( void )
{
   CNet_Disconnect( );
}
