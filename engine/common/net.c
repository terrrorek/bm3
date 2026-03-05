#include <common/net.h>
#include <common/cmd.h>

#include <string.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <WinSock2.h>
#  define WINERR  WSAGetLastError()
#else
#  include <errno.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  define WINERR  errno
#  define closesocket close
#endif

#ifdef _WIN32
static WSADATA wsadata;
#else
#endif

void Net_Init( void )
{
   int err;
#ifdef _WIN32
   err = WSAStartup( MAKEWORD(2, 2), &wsadata );
   if( err != NO_ERROR )
      Cmd_Errorf( "winsock initialization error (%i)\n", err );
#else
#endif
}

void Net_Quit( void )
{
#ifdef _WIN32
   WSACleanup( );
#else
#endif
}

void Netchan_Init( netchan_t* ch, netchkind_t kind, byte* addr, uint16_t port )
{
   int err;

   WIPE( *ch );
   ch->kind = kind;
   ch->port = port;
   memcpy( ch->addr, addr, 4 );

   ch->loopback = Mem_Alloc( NETCHAN_LOOPBACK_SIZE );
   ch->loopback_bytes = 0;

   switch( ch->kind )
   {
   case NETCHAN_LOOPBACK:
      break;
   case NETCHAN_NET:
      ch->handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
      err = WINERR;
      if( err != 0 )
         Cmd_Errorf( "failed to open netchan (%i)\n", err );
      break;
   }
}
void Netchan_Quit( netchan_t* ch )
{
   Mem_Free( ch->loopback );

   switch( ch->kind )
   {
   case NETCHAN_LOOPBACK:
      break;
   case NETCHAN_NET:
      closesocket( ch->handle );
      break;
   }
   WIPE( *ch );
}
