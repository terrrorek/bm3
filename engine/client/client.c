#include <client/client.h>
#include <server/server.h>

uint     cl_gametick = 0;

void CL_Init( void )
{

}

void CL_Quit( void )
{

}

void CL_Update( void )
{
   if( dedicated.data.integer != 0 )
      return;

}
