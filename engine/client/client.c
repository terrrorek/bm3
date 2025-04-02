
#include <client/client.h>
#include <common/cmd.h>
#include <host/host.h>

extern uint64_t secondDur;

void Client_Init( void )
{

}

void Client_Update( void )
{
   Ent_Process( true );
}

void Client_Quit( void )
{

}
