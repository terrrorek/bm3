#include <net/net.h>
#include <common/cmd.h>

#ifdef _WIN32

WSADATA wsaData;

static void Net_InitWinSock( void )
{
   int err;

   err = WSAStartup( MAKEWORD(2, 0), &wsaData );
   if( err )
   {
      Cmd_Errorf( "WSAStartup failed (%08x)\n", err );
   }
}
static void Net_QuitWinSock( void )
{
   WSACleanup( );
}

#endif

void Net_Init( void )
{
#ifdef _WIN32
   Net_InitWinSock( );
#endif
}

void Net_Quit( void )
{
#ifdef _WIN32
   Net_QuitWinSock( );
#endif
}
